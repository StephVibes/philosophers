/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 11:33:11 by smanriqu          #+#    #+#             */
/*   Updated: 2024/10/08 11:33:13 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	setting_tbl(t_tbl *tbl, char **argv, int argc)
{
	int	i;

	i = -1;
	tbl->num_of_philo = ft_atoll(argv[1], tbl);
	tbl->ttd = ft_atoll(argv[2], tbl);
	tbl->tte = ft_atoll(argv[3], tbl);
	tbl->tts = ft_atoll(argv[4], tbl);
	tbl->tme = -1;
	if (argc == 6)
		tbl->tme = ft_atoll(argv[5], tbl);
	tbl->phls = malloc(sizeof(t_phl) * tbl->num_of_philo);
	tbl->forks = malloc(sizeof(pthread_mutex_t) * tbl->num_of_philo);
	if (!tbl->forks || !tbl->phls)
		exit_error("Error: Malloc failed\n");
	while (++i < tbl->num_of_philo)
		pthread_mutex_init(&tbl->forks[i], NULL);
	i = -1;
	pthread_mutex_init(&tbl->print, NULL);
	pthread_mutex_init(&tbl->death, NULL);
	pthread_mutex_init(&tbl->flags, NULL);
	tbl->philo_died = 0;
	tbl->all_ate = 0;
	gettimeofday(&tbl->start, NULL);
}

void	init_philos(t_tbl *tbl)
{
	int	i;

	i = -1;
	while (++i < tbl->num_of_philo)
	{
		tbl->phls[i].id = i + 1;
		tbl->phls[i].lf = i;
		tbl->phls[i].rf = (i + 1) % tbl->num_of_philo;
		tbl->phls[i].te = 0;
		tbl->phls[i].le = get_current_time();
		tbl->phls[i].tbl = tbl; //TODO: check if this is necessary
	}
}

void	start_philosophers(t_tbl *tbl)
{
	int	i;

	i = -1;
	if (pthread_create(&tbl->monitor_thr, NULL, &monitor, tbl) != 0)
		destroy_mutex(tbl);
	while (++i < tbl->num_of_philo)
	{
		if (pthread_create(&tbl->phls[i].thread, NULL, &routine,
				&tbl->phls[i]) != 0)
			destroy_mutex(tbl);
	}
	i = -1;
	while (++i < tbl->num_of_philo)
	{
		if (pthread_join(tbl->phls[i].thread, NULL) != 0)
			destroy_mutex(tbl);
	}
	if (pthread_join(tbl->monitor_thr, NULL) != 0)
		destroy_mutex(tbl);
}

void	destroy_mutex(t_tbl *tbl)
{
	int	i;

	i = -1;
	while (++i < tbl->num_of_philo)
		pthread_mutex_destroy(&tbl->forks[i]);
	pthread_mutex_destroy(&tbl->print);
	pthread_mutex_destroy(&tbl->death);
	pthread_mutex_destroy(&tbl->flags);
	free(tbl->forks);
	free(tbl->phls);
	free(tbl);
	exit(1);
}
