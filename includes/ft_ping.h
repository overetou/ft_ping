//
// Created by Oscar VERETOUT on 3/26/21.
//

#ifndef FT_PING
#define FT_PING

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct  s_master
{
	const char          *target_domain_name;
	unsigned int        transmitted;
	unsigned int        received;
	long unsigned int   time;   //In ms.
}               t_master;

#endif //FT_PING
