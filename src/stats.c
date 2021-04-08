#include "ft_ping.h"

//TODO: secure the realloc in update_stats

void    update_stats(t_networking *n, t_master *m)
{
    if (m->received != 1)
    {
        if (n->time_diff < m->min)
            m->min = n->time_diff;
        else if (n->time_diff > m->max)
            m->max = n->time_diff;       
    }
    else
    {
        m->min = n->time_diff;
        m->max = n->time_diff;
    }
    m->results = realloc(m->results, m->received * sizeof(long int));
    m->results[(m->received) - 1] = n->time_diff;
}

//TODO: Manually check that the results of mean and mdev are correct (from the real ping and ours)

void    calculate_loss_percentage(t_master *m)
{
    if (m->received)
        m->packet_loss = 100 - (10000 / ((m->transmitted) * 100 / (m->received)));
    else
        m->packet_loss = 100;
}

void    calculate_mean(t_master *m)
{
    suseconds_t     sum = 0;
    unsigned int    i = 0;

    while (i != m->received)
    {
        sum += m->results[i];
        i++;
    }
    if (m->received)
        m->mean = sum / m->received;
}

suseconds_t    square_root(suseconds_t val)
{
    suseconds_t candidate = 0;

    while (candidate * candidate < val)
        candidate++;
    return candidate - 1;    
}

void    calculate_mdev(t_master *m)
{
    suseconds_t     squared_diff_sum = 0;
    unsigned int    i = 0;

    while (i != m->received)
    {
        squared_diff_sum += ((m->results[i]) - (m->mean)) * ((m->results[i]) - (m->mean));
        i++;
    }
    if (m->received)
        m->mdev = square_root(squared_diff_sum / m->received);
}