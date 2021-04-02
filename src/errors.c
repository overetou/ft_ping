#include "ft_ping.h"
#include <string.h>//TODO: Remove this line
#include <errno.h>//TODO: Remove this line

void critical_check(bool val, const char *msg)
{
	if (val)
		return;
	fprintf(stderr, "System report: \"%s\".\n", strerror(errno));
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(0);
}
