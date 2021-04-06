//
// Created by Oscar VERETOUT on 3/26/21.
//

#ifndef FT_PING
#define FT_PING

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <netdb.h>
#include <sys/time.h>
#define PACKET_SIZE 64
#define PORT_NO 0
#define PING_SLEEP_RATE 1000000 x
#define TIMEOUT_IN_SEC 1
#define REQ_SIZE 8

typedef struct  s_master
{
	const char          *destination;
	unsigned int        transmitted;
	unsigned int        received;
	struct timeval		time;
	struct timeval		stop_time;

	bool    			ping_loop;
	int                 domain;
	long int			min;
	long int			mean;
	long int			max;
	long int			mdev;
	suseconds_t			packet_loss;
	long int			*results;
	bool                verbose;
}               t_master;

typedef struct  s_networking
{
	char			reverse_addr[INET_ADDRSTRLEN];
	const char		*adr_nb;
	int				sd;
	struct icmphdr	req;
	struct addrinfo	*res;
	struct addrinfo	hints;
	struct timeval	time_save;
	struct timeval	second_time_save;
	long int		time_diff;
	char 			buffer[548];
	struct 			sockaddr_storage src_addr;
	struct 			iovec iov[1];
	struct			msghdr msg;
}               t_networking;

typedef struct  s_arg_processing
{
	int     argc;
	char    **argv;
	int     pos;
}               t_arg_processing;

t_master m;

bool 		ft_strcmp(char *s1, char *s2);
void		ft_strcpy(const char *src, char *dest);
void		ft_strncpy(const char *src, char *dest, int len);
void		get_time(struct timeval *tv);
long int	get_millisec_time_diff(struct timeval *before, struct timeval *after);
long int	get_microsec_time_diff(struct timeval *before, struct timeval *after);
void		wait_one_sec(void);
void 		critical_check(bool val, const char *msg);
void		update_stats(t_networking *n, t_master *m);
void    	calculate_mean(t_master *m);
void   		calculate_mdev(t_master *m);
void    	calculate_loss_percentage(t_master *m);
void 		print_conclusion(t_master *m);
void	   sig_handler(int sig);

#endif //FT_PING
