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

void establish_connection(t_master *m)
{
	t_networking n;

	n.socket_fd = socket(m->domain, SOCK_DGRAM, 0);//TODO: Study attentively
	// all of the unix network conventions.
	critical_check(n.socket_fd != -1, "Unable to create a socket.");
}
