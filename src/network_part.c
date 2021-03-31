//
// Created by Oscar VERETOUT on 3/28/21.
//
#include "ft_ping.h"

#include <string.h>//TODO: Remove this line
#include <errno.h>//TODO: Remove this line

void critical_check(bool val, const char *msg)
{
	if (val)
		return;
	fprintf(stderr, "System report: \"%s\".\n", strerror(errno));
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(0);
}

unsigned short checksum(void *b, int len)
{
	unsigned short *buf = b;
	unsigned int sum = 0;
	unsigned short result;

	while (len > 1)
	{
		sum += *buf;
		buf++;
		len -= 2;
	}
	if (len == 1)
		sum += *(unsigned char*)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}

uint16_t	ip_checksum(void *vdata, size_t len)
{
	char	*casted = (char*)vdata;
	uint32_t	accumulator = 0xffff;
	uint16_t	word;

	for (size_t i = 0; i + 1 < len; i += 2)
	{
		ft_strncpy(casted + i, (char*)(&word), 2);
		accumulator += ntohs(word);
		if (accumulator > 0xffff)
			accumulator -= 0xffff;
	}
	if (len & 1)
	{
		word = 0;
		ft_strncpy(casted + len - 1, (char*)(&word), 1);
		accumulator += ntohs(word);
		if (accumulator > 0xffff)
			accumulator -= 0xffff;
	}
	return htons(~accumulator);
}

void	set_sock_addr_in(struct sockaddr_in *a_in)
{
	a_in->sin_family = AF_INET;
}

void	open_socket(t_networking *n, t_master *m)
{
	n->sd = socket(m->domain, SOCK_RAW, IPPROTO_ICMP);
	critical_check(
		n->sd != -1,
		"Unable to create a socket."
	);
}

void	set_socket_options(t_networking *n, t_master *m)
{
	int				hdrincl = 0;
	critical_check(
		setsockopt(n->sd, IPPROTO_IP, IP_HDRINCL, &hdrincl, sizeof(hdrincl)) != -1,
		"setsockopt failure."
	);
}

//TODO see what htons do (and remove / replace it).

void	create_echo_request(t_networking *n, t_master *m)
{
	n->req.type = 8;
	n->req.code = 0;
	n->req.checksum = 0;
	n->req.un.echo.id = htons(rand());
	n->req.un.echo.sequence = htons(1);
	n->req.checksum = ip_checksum(&(n->req), REQ_SIZE);
}

void	convert_text_addr_to_struct(t_networking *n, t_master *m)
{
	n->hints.ai_addr = NULL;
	n->hints.ai_addrlen = 0;
	n->hints.ai_canonname = NULL;
	n->hints.ai_family = PF_UNSPEC;
	n->hints.ai_flags = 0 | AI_CANONNAME;
	n->hints.ai_next = NULL;
	n->hints.ai_protocol = 0;
	n->hints.ai_socktype = SOCK_STREAM;
	critical_check(
		getaddrinfo("google.com", NULL, &(n->hints), &(n->res)) == 0,
		"getaddrinfo failed.");
}

suseconds_t	get_ms(void)
{
	struct timeval	tv;
	struct timezone	tz;

	critical_check(
		gettimeofday(&tv, &tz) != -1,
		"Could not get time of day.");
	return tv.tv_usec;
}

void	setup_msg_getter(t_networking *n)
{
	n->iov[0].iov_base = n->buffer;
	n->iov[0].iov_len = sizeof(n->buffer);
	n->msg.msg_name = &(n->src_addr);
	n->msg.msg_namelen = sizeof(n->src_addr);
	n->msg.msg_iov = n->iov;
	n->msg.msg_iovlen = 1;
	n->msg.msg_control = 0;
	n->msg.msg_controllen = 0;
}

void	get_reply(t_networking *n, t_master *m)
{
	int reveived_len = recvmsg(n->sd, &(n->msg), 0);
	n->second_ms = get_ms();
	critical_check(reveived_len != -1, "Could not receive message proprely.");
	if (n->msg.msg_flags & MSG_TRUNC)
		puts("Message was too big for buffer. It was truncated.");
	struct sockaddr_in *addr = (struct sockaddr_in*)(n->res->ai_addr);
	n->second_ms -= n->ms_save;
	printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%ld.%ldms\n",
	reveived_len,
	"REPLACE_ME",
	inet_ntoa((struct in_addr)(addr->sin_addr)),
	0,
	0,
	(n->second_ms) / 1000,
	(n->second_ms - ((n->second_ms / 1000) * 1000)) / 10);
}

//TODO: change the size if need be for ipv6. l17
//TODO: if the protocol is ipv6, change the Type of the message. l19
// accordingly(https://en.wikipedia.org/wiki/Ping_(networking_utility)#ICMP_packet)
//TODO: Change AF_INET to AFINET6 when using ipv6. l29

//TODO : replace call to inet_itoa to homemade func.

void	print_introduction(t_networking *n)
{
	struct sockaddr_in *addr = (struct sockaddr_in*)(n->res->ai_addr);
	printf("PING %s (%s) 56(84) bytes of data.\n", "google.com", inet_ntoa((struct in_addr)(addr->sin_addr)));
}

void ping_periodicaly(t_master *m)
{
	t_networking n;

	n.ping_loop = true;
	open_socket(&n, m);
	set_socket_options(&n, m);
	create_echo_request(&n, m);
	setup_msg_getter(&n);
	convert_text_addr_to_struct(&n, m);
	print_introduction(&n);
	critical_check(
		sendto(n.sd, &(n.req), REQ_SIZE, 0, n.res->ai_addr, n.res->ai_addrlen) != -1,
		"sendto() failed.");
	n.ms_save = get_ms();
	get_reply(&n, m);
	freeaddrinfo(n.res);
	close(n.sd);
}