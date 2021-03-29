//
// Created by Oscar VERETOUT on 3/28/21.
//
#include "ft_ping.h"

void critical_check(bool val, const char *msg)
{
	if (val)
		return;
	puts(msg);
	exit(0);
}

//TODO: change the size if need be for ipv6. l17
//TODO: if the protocol is ipv6, change the Type of the message. l19
// accordingly(https://en.wikipedia.org/wiki/Ping_(networking_utility)#ICMP_packet)
//TODO: Change AF_INET to AFINET6 when using ipv6. l29

void establish_connection(t_master *m)
{
	t_networking n;
	n.ping_loop = true;
	char destination_bin[sizeof(struct in_addr)];
	char         echo_request[] = {
			8, 0, 0, 0,
			3, 232,
	};

	n.socket_fd = socket(m->domain, SOCK_DGRAM, 0);
	critical_check(n.socket_fd != -1, "Unable to create a socket.");
	//sendto(n.socket_fd, )
	inet_pton(AF_INET, "google.com", destination_bin);
}
