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
		fputs("usage: ft_ping [-AaDdfnoQqRrv] [-c count] [-G sweepmaxsize]\n"
		      "               [-g sweepminsize] [-h sweepincrsize] [-i wait]\n"
		      "               [-l preload] [-M mask | time] [-m ttl] [-p "
		      "pattern]\n"
		      "               [-S src_addr] [-s packetsize] [-t timeout][-W "
		      "waittime]\n"
		      "               [-z tos] host\n"
		      "       ft_ping [-AaDdfLnoQqRrv] [-c count] [-I iface] [-i "
		      "wait]\n"
		      "               [-l preload] [-M mask | time] [-m ttl] [-p "
		      "pattern] [-S src_addr]\n"
		      "               [-s packetsize] [-T ttl] [-t timeout] [-W "
		      "waittime]\n"
		      "               [-z tos] mcast-group\n", stderr);
		exit(0);
	}
	m->target_domain_name = argv[1];
}
