/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 11:33:26 by smanriqu          #+#    #+#             */
/*   Updated: 2024/10/08 11:33:28 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	print_status(t_phl *philo, char *status)
{
	t_tbl	*tbl;

	tbl = philo->tbl;
	pthread_mutex_lock(&tbl->flags);
	if ((tbl->philo_died || tbl->all_ate))
	{
		pthread_mutex_unlock(&tbl->flags);
		return ;
	}
	pthread_mutex_unlock(&tbl->flags);
	pthread_mutex_lock(&tbl->print);
	printf("%lld %d %s\n", time_elapsed(tbl->start), philo->id, status);
	pthread_mutex_unlock(&tbl->print);
}

static int	check_philo_death(t_tbl *tbl)
{
	int	i;

	i = -1;
	
	while (++i < tbl->num_of_philo)
	{
		if (get_current_time() - tbl->phls[i].le > tbl->ttd)
		{
			pthread_mutex_lock(&tbl->flags);
			tbl->philo_died = 1;
			pthread_mutex_unlock(&tbl->flags);
			pthread_mutex_lock(&tbl->print);
			printf("%lld %d %s\n", time_elapsed(tbl->start), tbl->phls[i].id, "died");
			pthread_mutex_unlock(&tbl->print);
			return (1);
		}
	}
	return (0);
}

static int	check_all_philos_ate_enough(t_tbl *tbl)
{
	int	i;

	if (tbl->tme == -1)
		return (0);
	i = -1;
	while (++i < tbl->num_of_philo)
	{
		if (tbl->phls[i].te < tbl->tme)
			return (0);
	}
	pthread_mutex_lock(&tbl->flags);
	tbl->all_ate = 1;
	pthread_mutex_unlock(&tbl->flags);
	return (1);
}

void	*monitor(void *arg)
{
	t_tbl	*tbl;

	tbl = (t_tbl *)arg;
	while (1)
	{
		pthread_mutex_lock(&tbl->death);
		if (check_philo_death(tbl))
		{
			pthread_mutex_unlock(&tbl->death);
			return (NULL);
		}
		if (check_all_philos_ate_enough(tbl))
		{
			pthread_mutex_unlock(&tbl->death);
			return (NULL);
		}
		pthread_mutex_unlock(&tbl->death);
		ft_usleep(500LL);
	}
}
