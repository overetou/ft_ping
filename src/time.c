#include "ft_ping.h"

time_t	get_sec(void)
{
	struct timeval	tv;
	struct timezone	tz;

	critical_check(
		gettimeofday(&tv, &tz) != -1,
		"Could not get time of day in seconds.");
	return tv.tv_sec;
}

void	wait_one_sec(void)
{
	time_t start = get_sec();
	
	while (get_sec() - start < 1);
}

suseconds_t	get_ms(void)
{
	struct timeval	tv;
	struct timezone	tz;

	critical_check(
		gettimeofday(&tv, &tz) != -1,
		"Could not get time of day in micro seconds.");
	return tv.tv_usec;
}
