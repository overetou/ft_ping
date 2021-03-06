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

void	set_sock_addr_in(struct sockaddr_in *a_in)
{
	a_in->sin_family = m.domain;
}

void	open_socket(t_networking *n, t_master *m)
{
	n->sd = socket(m->domain, SOCK_RAW, (m->domain == AF_INET6 ? IPPROTO_ICMPV6 : IPPROTO_ICMP));
	critical_check(
		n->sd != -1,
		"Unable to create a socket. Did you launch the command as the superuser?"
	);
}

void	set_socket_options(t_networking *n, t_master *m)
{
	int				hdrincl = 0;
	struct timeval	tv = {1, 0};//This will set the timeout of a ping to 1 second.

	if (m->domain == AF_INET)
	{
		critical_check(
			setsockopt(n->sd, IPPROTO_IP, IP_HDRINCL, &hdrincl, sizeof(hdrincl)) != -1,
			"setsockopt failure on ip header inclusion."
		);
	}
	else
	{
		critical_check(
			setsockopt(n->sd, IPPROTO_IPV6, IPV6_HDRINCL, &hdrincl, sizeof(hdrincl)) != -1,
			"setsockopt failure on ipv6 header inclusion."
		);
	}
	critical_check(
		setsockopt(n->sd, SOL_SOCKET, SO_RCVTIMEO, (const char*)(&tv), sizeof(struct timeval)) != 1,
		"setsockopt failure on setting timeout value for receiving package."
	);
}

void	charcpyn(char *dest, const char *src)
{
	int i = 0;

	while (src[i])
	{
		dest[i] = src[i];
		i++;
	}
}

void	create_echo_request(t_networking *n)
{	
	n->req = (struct icmphdr*)(n->req_buffer);
	n->req->type = ICMP_ECHO;
	n->req->code = 0;
	n->req->checksum = 0;
	n->req->un.echo.id = getuid();
	n->req->un.echo.sequence = 1;
	charcpyn((n->req_buffer) + REQ_SIZE + TIME_STAMP_SIZE, "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuv");
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
	if (getaddrinfo(m->destination, NULL, &(n->hints), &(n->res)) != 0)
	{
		printf("ft_ping: %s: Nom ou service inconnu\n", m->destination);
		exit(0);
	}
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

//Are we reading at the good spot in IPV6 mode?
void	get_ttl(t_networking *n)
{
	struct iphdr *iph = (struct iphdr*)(n->buffer);
	n->reply_ttl = iph->ttl;
}

void	get_reply(t_networking *n, t_master *m)
{
	int received_len = recvmsg(n->sd, &(n->msg), 0);
	get_time(&(n->second_time_save));
	get_ttl(n);
	if (received_len == -1)
	{
		if (m->verbose == true)
			puts("Awaited reply timed out.");
		return;
	}
	(m->received)++;
	if (n->msg.msg_flags & MSG_TRUNC)
		puts("Message was too big for buffer. It was truncated.");
	n->time_diff = get_microsec_time_diff(&(n->time_save), &(n->second_time_save));
	printf("%d bytes from %s (%s): icmp_seq=%d ttl=%d time=%ld.%ldms\n",
	received_len != 0 ? received_len - 20 : received_len,
	n->res->ai_canonname,
	inet_ntop(m->domain,
	&(((struct sockaddr_in*)(n->res->ai_addr))->sin_addr),
	n->reverse_addr,
	INET_ADDRSTRLEN),
	m->received,
	n->reply_ttl,
	(n->time_diff) / 1000,
	(n->time_diff - ((n->time_diff / 1000) * 1000)) / 10);
	update_stats(n, m);
}

void	print_introduction(t_networking *n, t_master *m)
{
	printf("PING %s (%s) 56(84) bytes of data.\n", m->destination,
	inet_ntop(m->domain,
	&(((struct sockaddr_in*)(n->res->ai_addr))->sin_addr),
	n->reverse_addr,
	INET_ADDRSTRLEN));
}

void	ping(t_networking *n, t_master *m, int *loop_nb)
{
    (m->transmitted)++;
	update_request_time_stamp(n);
	critical_check(
		sendto(n->sd, n->req, REQ_SIZE + TIME_STAMP_SIZE + DATA_SIZE,
		0, n->res->ai_addr, n->res->ai_addrlen) != -1,
		"sendto() failed.");
	get_time(&(n->time_save));
	get_reply(n, m);
	(*loop_nb)++;
}

void ping_periodicaly(t_master *m)
{
	t_networking n;
	int loop_nb = 0;

	open_socket(&n, m);
	set_socket_options(&n, m);
	create_echo_request(&n);
	setup_msg_getter(&n);
	convert_text_addr_to_struct(&n, m);
	print_introduction(&n, m);
	get_time(&(m->timer_start));
	ping(&n, m, &loop_nb);
	get_time(&(m->time));
	get_time(&(m->stop_time));
	wait_one_sec(loop_nb);
	while (m->ping_loop == true && loop_nb != m->loop_nb)
	{
		get_time(&(m->timer_start));
		ping(&n, m, &loop_nb);
		get_time(&(m->stop_time));
		wait_one_sec(loop_nb);
	}
	freeaddrinfo(n.res);
	close(n.sd);
}