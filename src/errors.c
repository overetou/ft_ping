#include "ft_ping.h"

void	critical_check(bool val, const char *msg)
{
	if (val)
		return;
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(0);
}

void	*secure_realloc(void *old_adr, size_t len)
{
	void	*new = realloc(old_adr, len);
	
	critical_check(new != NULL,
	"Realloc failed: no more memory available, invalid alloc length or memory corrupted.");
	return new;
}
