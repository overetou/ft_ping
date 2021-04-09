#include "ft_ping.h"

void critical_check(bool val, const char *msg)
{
	if (val)
		return;
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(0);
}
