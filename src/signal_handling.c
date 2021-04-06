#include "ft_ping.h"

void    sig_handler(int sig)
{
    m.ping_loop = false;
    puts("");
}