#include "../include/philo.h"

void	setting_tbl(t_tbl *tbl, char **argv, int argc);
void	init_philos(t_tbl *tbl);
void	start_philosophers(t_tbl *tbl);

void	start_tbl(t_tbl *tbl, char **argv, int argc)
{
	setting_tbl(tbl, argv, argc);
	init_philos(tbl);
	start_philosophers(tbl);
}

void	setting_tbl(t_tbl *tbl, char **argv, int argc)
{
	int	i;

	i = -1;
	tbl->num_of_philo = ft_atol(argv[1]);
	tbl->ttd = ft_atol(argv[2]);
	tbl->tte = ft_atol(argv[3]);
	tbl->tts = ft_atol(argv[4]);
	tbl->tme = -1;
	if (argc == 6)
		tbl->tme = ft_atol(argv[5]);
	tbl->phls = malloc(sizeof(t_phl) * tbl->num_of_philo);
	tbl->forks = malloc(sizeof(pthread_mutex_t) * tbl->num_of_philo);
	if (!tbl->forks || !tbl->phls)
		exit_error("Error: Malloc failed\n");
	while (++i < tbl->num_of_philo)
		pthread_mutex_init(&tbl->forks[i], NULL);
	i = -1;
	pthread_mutex_init(&tbl->print, NULL);
	pthread_mutex_init(&tbl->ready_mtx, NULL);
	tbl->philo_died = 0;
	tbl->ready = 0;
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
		tbl->phls[i].tbl = tbl;
	}
}

void	start_philosophers(t_tbl *tbl)
{
	int i;

	i = -1;
	while (++i < tbl->num_of_philo)
	{
		if (pthread_create(&tbl->phls[i].thread, NULL, &dinner_routine,
				&tbl->phls[i]) != 0)
			exit_error("Error: Failed to create philosopher thread\n");
	}
	gettimeofday(&tbl->start, NULL);
	tbl->ready = 1;
	if(pthread_create(&tbl->check_death, NULL, &monitor, tbl) != 0)
		exit_error("Error: Failed to create check death thread\n");
	i = -1;
	if (pthread_join(tbl->check_death, NULL) != 0)
		exit_error("Error: Failed to join check death thread\n");
	while (++i < tbl->num_of_philo)
	{
		if (pthread_join(tbl->phls[i].thread, NULL) != 0)
			exit_error("Error: Failed to join philosopher thread\n");
	}
}
