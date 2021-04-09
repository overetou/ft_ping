//
// Created by Oscar VERETOUT on 3/28/21.
//

#include "ft_ping.h"

void process_arguments(int argc, char **argv, t_master *m)
{
	t_arg_processing ap;

	setup_arg_processing(argc, argv, &ap);
	check_explicit_ipv_version(&ap, m);
	process_flags(&ap, m);
	m->destination = argv[ap.pos];
}

bool	stouint(const char *str, int *to_fill)
{
	int i = 0;
	*to_fill = 0;

	while (str[i])
	{
		*to_fill *= 10;
		if (str[i] < '0' || str[i] > '9')
			return (false);
		*to_fill += str[i] - '0';
		i++;
	}
	return (i != 0 && *to_fill > 0 ? true : false);
}

bool	try_c_flag(t_arg_processing *ap, t_master *m, unsigned short i)
{
	if (ap->argv[ap->pos][i] == 'c')
	{
		critical_check(
			m->loop_nb == -1,
			"-c argument cannot be specified more than one time.");
		critical_check(
			ap->argv[ap->pos][i + 1] == '\0',
			"-c must be directly folowed by its value.");
		(ap->pos)++;
		check_coherent_pos(ap);
		critical_check(stouint(
			ap->argv[ap->pos], &(m->loop_nb)) == true,
			"Illegal value for -c. An absolute and non zero integer value was expected.");
		(ap->pos)++;
		return (true);
	}
	return (false);
}

void process_flags(t_arg_processing *ap, t_master *m)
{
	unsigned short i;

	check_coherent_pos(ap);
	while (ap->argv[ap->pos][0] == '-')
	{
		i = 1;
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
						fputs("-c must be used alone.\n", stderr);
					else
						fprintf(stderr, "Unknown parameter '-%c'.\n", ap->argv[ap->pos][i]);
					exit(0);
				}
				i++;
			}
			(ap->pos)++;
		}
		check_coherent_pos(ap);
	}
}

void check_coherent_pos(t_arg_processing *ap)
{
	critical_check(ap->pos != ap->argc,
	"Incomplete statement detected. Use ft_ping -h to display usage.");
}

void display_usage()
{
	puts("Usage: ft_ping [-4/-6] [-vh] [-c <packet number>] destination\n"
		"\t-4: use ipv4. -6: use ipv6 (non functionnal). Default: ipv4."
		" This must be set before anything else.\n"
		"\t-v: verbose upon package loss.\n"
		"\t-h: display this usage\n"
		"\t-c <packet number>: Stop looping after <packet number> packets have been sent.\n"
		"Destination can be a domain name (ex: wikipedia.org) or an ip (ex: 91.198.174.192).\n"
		"Arguments beyond the destination will not be be processed.");
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
