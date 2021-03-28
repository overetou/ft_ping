#include "includes/ft_ping.h"

void process_arguments(int argc, char **argv, t_master *m);

void setup_master(t_master *m);

void print_conclusion(t_master *m);

int main(int argc, char **argv)
{
	t_master m;

	process_arguments(argc, argv, &m);
	setup_master(&m);
	print_conclusion(&m);
	return 0;
}

void print_conclusion(t_master *m)
{
	printf("--- %s ping statistics ---\n"
	       "%u packets transmitted, %u packets received, %.1f%% packet loss\n",
	       m->target_domain_name, m->transmitted, m->received, 0.0);
}

void setup_master(t_master *m)
{
	m->transmitted = 0;
	m->received = 0;
}

void process_arguments(int argc, char **argv, t_master *m)
{
	if (argc != 2) {
		puts("Usage: ft_ping [-vh] destination\n"
	   "Usage: ft_ping -6 [-vh] destination");
		exit(0);
	}
	m->target_domain_name = argv[1];
}