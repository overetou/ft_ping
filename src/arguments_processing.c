//
// Created by Oscar VERETOUT on 3/28/21.
//

#include "ft_ping.h"

void setup_arg_processing(t_arg_processing *ap);

void check_explicit_ipv_version(int argc, char **argv, t_arg_processing *ap,
                                t_master *p_master);

void display_usage();

void process_arguments(int argc, char **argv, t_master *m)
{
	t_arg_processing ap;

	if (argc != 2) {
		display_usage();
	}
	setup_arg_processing(&ap);
	check_explicit_ipv_version(argc, argv, &ap, m);
	m->destination = argv[1];
}

void display_usage()
{
	puts("Usage: ft_ping [-vh] destination\n"
	     "Usage: ft_ping -6 [-vh] destination");
	exit(0);
}

void check_explicit_ipv_version(int argc, char **argv, t_arg_processing *ap,
                                t_master *m)
{
	if (ft_strcmp(argv[ap->pos], "-4"))
		m->ipv_version = IPV4;
	else if (ft_strcmp(argv[ap->pos], "-6"))
		m->ipv_version = IPV6;
	else
		return;
	ap->pos++;
}

void setup_arg_processing(t_arg_processing *ap)
{
	ap->pos = 1;
}
