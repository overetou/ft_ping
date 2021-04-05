#include "ft_ping.h"

//TODO: secure the realloc in update_stats

void    update_stats(t_networking *n, t_master *m)
{
    if (m->transmitted)
    {
        if (n->second_time_save < m->min)
            m->min = n->second_time_save;
        else if (n->second_time_save > m->max)
            m->max = n->second_time_save;       
    }
    else
    {
        m->min = n->second_time_save;
        m->max = n->second_time_save;
    }
    (m->transmitted)++;
    m->results = realloc(m->results, m->transmitted * sizeof(struct timeval));
    ft_strncpy((char*)(m->results + (m->transmitted) - 1), (char*)(&(n->second_time_save)), sizeof(struct timeval));
}

//TODO: Manually check that the results of mean and mdev are correct (from the real ping and ours)

void    calculate_loss_percentage(t_master *m)
{
    if (m->received)
        m->packet_loss = 100 - (100 / ((m->transmitted) / (m->received)));
    else
        m->packet_loss = 0;
}

void    calculate_mean(t_master *m)
{
    suseconds_t     sum = 0;
    unsigned int    i = 0;

    while (i != m->transmitted)
    {
        sum += m->results[i];
        i++;
    }
    m->mean = sum / m->transmitted;
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

    while (i != m->transmitted)
    {
        squared_diff_sum += ((m->results[i]) - (m->mean)) * ((m->results[i]) - (m->mean));
        i++;
    }
    m->mdev = square_root(squared_diff_sum / m->transmitted);
}