#include "ft_ping.h"
#include "signal.h"

void process_arguments(int argc, char **argv, t_master *m);

void setup_master(t_master *m);

void ping_periodicaly(t_master *m);

int main(int argc, char **argv)
{
	process_arguments(argc, argv, &m);
	setup_master(&m);
	signal(SIGINT, sig_handler);
	ping_periodicaly(&m);
	print_conclusion(&m);
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
	    get_millisec_time_diff(&(m->time), &(m->stop_time)),
		m->min / 1000, m->min - (m->min / 1000 * 1000),
		m->mean / 1000, m->mean - (m->mean / 1000 * 1000),
		m->max / 1000, m->max - (m->max / 1000 * 1000),
		m->mdev / 1000, m->mdev - (m->mdev / 1000 * 1000));
	if (m->received)
		free(m->results);
}

void setup_master(t_master *m)
{
	m->ping_loop = true;
	m->transmitted = 0;
	m->received = 0;
	m->verbose = false;
	m->mean = 0;
	m->mdev = 0;
	m->min = 0;
	m->max = 0;
	m->results = NULL;
}