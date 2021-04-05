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
	if (m.transmitted)
		free(m.results);
	return 0;
}

void print_conclusion(t_master *m)
{
	calculate_mean(m);
	calculate_mdev(m);
	calculate_loss_percentage(m);
	printf("--- %s ping statistics ---\n"
	    "%u packets transmitted, %u received, %ld%% packet loss, time "
		"%ldms\nrtt min/mean/max/mdev = %ld.%ld/%ld.%ld/%ld.%ld/%ld.%ld ms\n", m->destination, m->transmitted, m->received, m->packet_loss,
	    (m->stop_time - m->time),
		m->min / 1000, m->min - (m->min / 1000 * 1000),
		m->mean / 1000, m->mean - (m->mean / 1000 * 1000),
		m->max / 1000, m->max - (m->max / 1000 * 1000),
		m->mdev / 1000, m->mdev - (m->mdev / 1000 * 1000));
}

void setup_master(t_master *m)
{
	m->transmitted = 0;
	m->received = 0;
	m->verbose = false;
	m->mean = 0;
	m->results = NULL;
	m->transmitted = 0;
}