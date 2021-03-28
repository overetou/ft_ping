//
// Created by Oscar VERETOUT on 3/26/21.
//

#ifndef FT_PING
#define FT_PING

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

enum e_ipv_version {IPV4, IPV6, DEFAULT};

typedef struct  s_master
{
	const char          *destination;
	unsigned int        transmitted;
	unsigned int        received;
	long unsigned int   time;   //In ms.

	enum e_ipv_version  ipv_version;
	bool                verbose;
}               t_master;

typedef struct  s_arg_processing
{
	int     argc;
	char    **argv;
	int     pos;
}               t_arg_processing;

bool ft_strcmp(char *s1, char *s2);

#endif //FT_PING
