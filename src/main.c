#include "../include/philo.h"

void	cleanup(t_tbl *tbl)
{
	int	i;

	i = -1;
	while (++i < tbl->num_of_philo)
		pthread_mutex_destroy(&tbl->forks[i]);
	pthread_mutex_destroy(&tbl->print);
	free(tbl->forks);
	free(tbl->phls);
	free(tbl);
}

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
