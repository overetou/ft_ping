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

bool	stouint(const char *str)
{
	int i = 0;

	while (str[i])
	{
		
	}
	return (true);
}

bool	try_c_flag(t_arg_processing *ap, t_master *m, unsigned short i)
{
	if (ap->argv[ap->pos][i] == 'c')
	{
		critical_check(m->loop_nb == -1,
		"-c argument cannot be specified more than one time.");
		critical_check(ap->argv[ap->pos][i + 1] == '\0',
		"-c must be directly folowed by its value.");
		critical_check(stouint(ap->argv[ap->pos + 1], &(m->loop_nb)) == true,
		"Illegal value for -c.");
		return (true);
	}
	return (false);
}

void process_standalone_flags(t_arg_processing *ap, t_master *m)
{
	unsigned short i = 1;

	while (ap->argv[ap->pos][0] == '-')
	{
		check_coherent_pos(ap);
		if (try_c_flag(ap, m, i) == false)
		{
			while (ap->argv[ap->pos][i])
			{
				if (ap->argv[ap->pos][i] == 'v')
					m->verbose = true;
				else if (ap->argv[ap->pos][i] == 'h')
					display_usage();
				else
				{
					if (ap->argv[ap->pos][i] == 'c')
						fputs("-c must be used alone.", stderr);
					else
						fprintf(stderr, "Unknown parameter '-%c'.\n", ap->argv[ap->pos][i]);
					exit(0);
				}
				i++;
			}
			if (i == 1)
				display_usage();
			(ap->pos)++;
		}
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
