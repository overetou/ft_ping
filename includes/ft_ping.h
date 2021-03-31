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
	suseconds_t		   time;   //In ms.

	int                 domain;
	suseconds_t			min;
	suseconds_t			mean;
	suseconds_t			max;
	bool                verbose;
}               t_master;

typedef struct  s_networking
{
	bool    		ping_loop;
	int				sd;
	struct icmphdr	req;
	struct addrinfo	*res;
	struct addrinfo	hints;
	suseconds_t		ms_save;
	suseconds_t		second_ms;
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

bool ft_strcmp(char *s1, char *s2);
void	ft_strcpy(const char *src, char *dest);
void	ft_strncpy(const char *src, char *dest, int len);
suseconds_t	get_ms(void);

#endif //FT_PING
