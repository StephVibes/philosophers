/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 11:33:19 by smanriqu          #+#    #+#             */
/*   Updated: 2024/10/08 11:33:22 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	main(int argc, char *argv[])
{
	t_tbl	*tbl;

	if (argc < 5 || argc > 6)
	{
		instructions();
		exit_error("Error: Invalid number of arguments\n");
	}
	tbl = malloc(sizeof(t_tbl));
	if (!tbl)
		exit_error("Error: Malloc failed\n");
	setting_tbl(tbl, argv, argc);
	init_philos(tbl);
	start_philosophers(tbl);
	cleanup(tbl);
	return (0);
}

void	cleanup(t_tbl *tbl)
{
	int	i;

	i = -1;
	while (++i < tbl->num_of_philo)
		pthread_mutex_destroy(&tbl->forks[i]);
	pthread_mutex_destroy(&tbl->print);
	pthread_mutex_destroy(&tbl->death);
	free(tbl->forks);
	free(tbl->phls);
	free(tbl);
}

void	instructions(void)
{
	printf("**************************************\n");
	printf("**Instructions**\n");
	printf("**Example:**\n");
	printf("  ./philo 5 800 200 200 7\n");
	printf("  This starts the simulation with 5 philosophers,");
	printf("where each can survive 800 ms without eating,\n");
	printf("  takes 200 ms to eat, sleeps for 200 ms,");
	printf("and each philosopher must eat 7 times.\n\n");
	printf("****************************************\n\n");
}
