/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 11:33:39 by smanriqu          #+#    #+#             */
/*   Updated: 2024/10/08 11:33:41 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*routine(void *arg)
{
	t_phl	*philo;
	t_tbl	*tbl;

	philo = (t_phl *)arg;
	tbl = philo->tbl;
	if (tbl->num_of_philo == 1)
	{
		pthread_mutex_lock(&tbl->forks[philo->rf]);
		print_status(philo, "has taken a fork");
		ft_usleep(tbl->ttd * 1000LL);
		pthread_mutex_unlock(&tbl->forks[philo->rf]);
		return (NULL);
	}
	while (1)
	{
		pthread_mutex_lock(&tbl->flags);
		if (tbl->philo_died == 1 || tbl->all_ate == 1)
		{
			pthread_mutex_unlock(&tbl->flags);
			break;
		}
		pthread_mutex_unlock(&tbl->flags);
		eating(philo);
		sleeping(philo);
	}
	return (NULL);
}

void	eating(t_phl *philo)
{
	t_tbl	*tbl;

	tbl = philo->tbl;
	take_forks(philo);
	print_status(philo, "is eating");
	pthread_mutex_lock(&tbl->death);
	philo->le = get_current_time();
	philo->te++;
	pthread_mutex_unlock(&tbl->death);
	ft_usleep(tbl->tte * 1000LL);
	pthread_mutex_unlock(&tbl->forks[philo->rf]);
	pthread_mutex_unlock(&tbl->forks[philo->lf]);
}

void	take_forks(t_phl *philo)
{
	t_tbl	*tbl;

	tbl = philo->tbl;
	if (philo->id % 2 != 0)
	{
		pthread_mutex_lock(&tbl->forks[philo->lf]);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&tbl->forks[philo->rf]);
		print_status(philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&tbl->forks[philo->rf]);
		print_status(philo, "has taken a fork");
		pthread_mutex_lock(&tbl->forks[philo->lf]);
		print_status(philo, "has taken a fork");
	}
}

void	sleeping(t_phl *philo)
{
	t_tbl		*tbl;
	long long	ttthink;

	tbl = philo->tbl;
	print_status(philo, "is sleeping");
	ft_usleep(tbl->tts * 1000LL);
	print_status(philo, "is thinking");
	if (philo->id % 2 == 0)
		ft_usleep(10LL);
	ttthink = tbl->ttd - tbl->tts - tbl->tte;
	if (ttthink < 0)
		ft_usleep(900LL);
	else
		ft_usleep(ttthink * 900LL);
}
