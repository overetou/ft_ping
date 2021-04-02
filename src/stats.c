#include "ft_ping.h"

//TODO: secure the realloc in update_stats

void    update_stats(t_networking *n, t_master *m)
{
    if (m->nb_results)
    {
        if (n->second_ms < m->min)
            m->min = n->second_ms;
        else if (n->second_ms > m->max)
            m->max = n->second_ms;       
    }
    else
    {
        m->min = n->second_ms;
        m->max = n->second_ms;
    }
    (m->nb_results)++;
    m->results = realloc(m->results, m->nb_results * sizeof(suseconds_t));
    m->results[(m->nb_results) - 1] = n->second_ms;
}

//TODO: Manually check that the results of mean and mdev are correct (from the real ping and ours)

void    calculate_mean(t_master *m)
{
    suseconds_t     sum = 0;
    unsigned int    i = 0;

    while (i != m->nb_results)
    {
        sum += m->results[i];
        i++;
    }
    m->mean = sum / m->nb_results;
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

    while (i != m->nb_results)
    {
        squared_diff_sum += ((m->results[i]) - (m->mean)) * ((m->results[i]) - (m->mean));
        i++;
    }
    m->mdev = square_root(squared_diff_sum / m->nb_results);
}