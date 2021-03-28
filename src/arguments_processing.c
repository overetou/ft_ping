//
// Created by Oscar VERETOUT on 3/28/21.
//

#include "ft_ping.h"

void setup_arg_processing(int argc, char **argv,
                          t_arg_processing *ap);

void check_explicit_ipv_version(t_arg_processing *ap, t_master *m);

void display_usage();

void check_coherent_pos(t_arg_processing *ap);

void process_arguments(int argc, char **argv, t_master *m)
{
	t_arg_processing ap;

	setup_arg_processing(argc, argv, &ap);
	check_explicit_ipv_version(&ap, m);
	check_coherent_pos(&ap);
	m->destination = argv[ap.pos];
}

void check_coherent_pos(t_arg_processing *ap)
{
	if (ap->pos == ap->argc)
		display_usage();
}

void display_usage()
{
	puts("Usage: ft_ping [-vh] destination\n"
	     "Usage: ft_ping -6 [-vh] destination");
	exit(0);
}

void check_explicit_ipv_version(t_arg_processing *ap, t_master *m)
{
	check_coherent_pos(ap);
	if (ft_strcmp(ap->argv[ap->pos], "-4"))
	{
		puts("Working with IPv4.");
		m->ipv_version = IPV4;
	}
	else if (ft_strcmp(ap->argv[ap->pos], "-6"))
	{
		puts("Working with IPv6.");
		m->ipv_version = IPV6;
	}
	else
	{
		puts("Working with default IPv.");
		m->ipv_version = DEFAULT;
		return;
	}
	(ap->pos)++;
}

void setup_arg_processing(int argc, char **argv, t_arg_processing *ap)
{
	ap->argc = argc;
	ap->argv = argv;
	ap->pos = 1;
}
