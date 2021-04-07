//
// Created by Oscar VERETOUT on 3/28/21.
//
#include "ft_ping.h"

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
		getaddrinfo(m->destination, NULL, &(n->hints), &(n->res)) == 0,
		"getaddrinfo failed.");
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

void	print_ttl(t_networking *n)
{
	struct iphdr *iph = (struct iphdr*)(n->buffer);
	n->reply_ttl = iph->ttl;
}

void	get_reply(t_networking *n, t_master *m)
{
	int reveived_len = recvmsg(n->sd, &(n->msg), 0);
	get_time(&(n->second_time_save));
	print_ttl(n);
	(m->received)++;
	critical_check(
		reveived_len != -1,
		"Could not receive message proprely.");
	if (n->msg.msg_flags & MSG_TRUNC)
		puts("Message was too big for buffer. It was truncated.");
	n->time_diff = get_microsec_time_diff(&(n->time_save), &(n->second_time_save));
	
	printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%ld.%ldms\n",
	reveived_len,
	n->res->ai_canonname,
	inet_ntop(AF_INET,
	&(((struct sockaddr_in*)(n->res->ai_addr))->sin_addr),
	n->reverse_addr,
	INET_ADDRSTRLEN),
	m->received,
	n->reply_ttl,
	(n->time_diff) / 1000,
	(n->time_diff - ((n->time_diff / 1000) * 1000)) / 10);
	update_stats(n, m);
}

//TODO: change the size if need be for ipv6. l17
//TODO: if the protocol is ipv6, change the Type of the message. l19
// accordingly(https://en.wikipedia.org/wiki/Ping_(networking_utility)#ICMP_packet)
//TODO: Change AF_INET to AFINET6 when using ipv6. l29

//TODO : replace call to inet_itoa to homemade func.

void	print_introduction(t_networking *n, t_master *m)
{
	printf("PING %s (%s) 56(84) bytes of data.\n", m->destination,
	inet_ntop(AF_INET,
	&(((struct sockaddr_in*)(n->res->ai_addr))->sin_addr),
	n->reverse_addr,
	INET_ADDRSTRLEN));
}

void	ping(t_networking *n, t_master *m)
{
	critical_check(
		sendto(n->sd, &(n->req), REQ_SIZE, 0, n->res->ai_addr, n->res->ai_addrlen) != -1,
		"sendto() failed.");
	get_time(&(n->time_save));
	get_reply(n, m);
}

void ping_periodicaly(t_master *m)
{
	t_networking n;

	open_socket(&n, m);
	set_socket_options(&n, m);
	create_echo_request(&n, m);
	setup_msg_getter(&n);
	convert_text_addr_to_struct(&n, m);
	print_introduction(&n, m);
	ping(&n, m);
	get_time(&(m->time));
	wait_one_sec();
	while (m->ping_loop == true)
	{	
		ping(&n, m);
		wait_one_sec();
	}
	get_time(&(m->stop_time));
	freeaddrinfo(n.res);
	close(n.sd);
}