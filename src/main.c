#include "ft_ping.h"

void process_arguments(int argc, char **argv, t_master *m);

void setup_master(t_master *m);

void print_conclusion(t_master *m);

void ping_periodicaly(t_master *m);

int main(int argc, char **argv)
{
	t_master m;

	process_arguments(argc, argv, &m);
	setup_master(&m);
	ping_periodicaly(&m);
	print_conclusion(&m);
	return 0;
}

void print_conclusion(t_master *m)
{
	printf("--- %s ping statistics ---\n"
	       "%u packets transmitted, %u received, %.1f%% packet loss, time "
		"%lums\n", m->destination, m->transmitted, m->received, 0.0,
	       m->time);
}

void setup_master(t_master *m)
{
	m->transmitted = 0;
	m->received = 0;
	m->time = 0;
	m->verbose = false;
}