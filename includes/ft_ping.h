//
// Created by Oscar VERETOUT on 3/26/21.
//

#ifndef FT_PING
#define FT_PING

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#define PACKET_SIZE 64
#define PORT_NO 0
#define PING_SLEEP_RATE 1000000 x
#define TIMEOUT_IN_SEC 1

typedef struct  s_master
{
	const char          *destination;
	unsigned int        transmitted;
	unsigned int        received;
	long unsigned int   time;   //In ms.

	int                 domain;
	bool                verbose;
}               t_master;

typedef struct	s_icmp_request_data
{
	char type;
	char code;
	
}				t_icmp_request_data;

typedef struct  s_networking
{
	bool    ping_loop;
	int     type;
	int     protocol;
	int     socket_fd;
	
	t_icmp_request_data	data;
}               t_networking;

typedef struct  s_arg_processing
{
	int     argc;
	char    **argv;
	int     pos;
}               t_arg_processing;

bool ft_strcmp(char *s1, char *s2);
void	ft_strcpy(const char *src, char *dest);

#endif //FT_PING
