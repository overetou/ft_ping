//
// Created by Oscar VERETOUT on 3/28/21.
//

#include "ft_ping.h"

void process_arguments(int argc, char **argv, t_master *m)
{
	t_arg_processing ap;

	setup_arg_processing(argc, argv, &ap);
	check_explicit_ipv_version(&ap, m);
	process_standalone_flags(&ap, m);
	check_coherent_pos(&ap);
	m->destination = argv[ap.pos];
}

void process_standalone_flags(t_arg_processing *ap, t_master *m)
{
	unsigned short i = 1;

	check_coherent_pos(ap);
	if (ap->argv[ap->pos][0] == '-')
	{
		while (ap->argv[ap->pos][i])
		{
			if (ap->argv[ap->pos][i] == 'v')
				m->verbose = true;
			else if (ap->argv[ap->pos][i] == 'h')
				display_usage();
			i++;
		}
		if (i == 1)
			display_usage();
		(ap->pos)++;
	}
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
		m->domain = AF_INET;
	else if (ft_strcmp(ap->argv[ap->pos], "-6"))
		m->domain = AF_INET6;
	else
	{
		m->domain = AF_INET;
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
