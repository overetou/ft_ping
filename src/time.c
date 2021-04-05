#include "ft_ping.h"

void	get_time(struct timeval *tv)
{
	struct timezone	tz;

	critical_check(
		gettimeofday(tv, &tz) != -1,
		"Could not get time of day in seconds.");
}

long int	get_millisec_time_diff(struct timeval *before, struct timeval *after)
{
	time_t 		tmp_sec;
	suseconds_t	tmp_microsec;
	long int	result;

	if (before->tv_sec == after->tv_sec)
		return (after->tv_usec - before->tv_usec) / 1000;
	tmp_sec = after->tv_sec - before->tv_sec;
	tmp_microsec = after->tv_usec - before->tv_usec;
	if (tmp_microsec < 0)
	{
		tmp_sec--;
		tmp_microsec = 1000000 + tmp_microsec;
	}
	result = tmp_sec * 1000;
	result += tmp_microsec / 1000;
	return result;
}

void	wait_one_sec(void)
{
	time_t start = get_sec();
	
	while (get_sec() - start < 1);
}
