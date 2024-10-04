#include "../include/philo.h"


void	drop_forks(t_phl *philo)
{
	t_tbl *tbl = philo->tbl;

	pthread_mutex_unlock(&tbl->forks[philo->rf]);
	pthread_mutex_unlock(&tbl->forks[philo->lf]);
}

void	take_forks(t_phl *philo)
{
	t_tbl *tbl = philo->tbl;

	if (pthread_mutex_lock(&tbl->forks[philo->rf]) != 0)
		exit_error("Error: Failed to lock right fork\n");
	pthread_mutex_lock(&tbl->print);
	printf("%d %d has taken a fork\n", get_current_dif_time(tbl->start),
		philo->id);
	pthread_mutex_unlock(&tbl->print);
	if (pthread_mutex_lock(&tbl->forks[philo->lf]) != 0)
		exit_error("Error: Failed to lock left fork\n");
	pthread_mutex_lock(&tbl->print);
	printf("%d %d has taken a fork\n", get_current_dif_time(tbl->start),
		philo->id);
	pthread_mutex_unlock(&tbl->print);
}

void	*check_if_philo_died(void *arg)
{
	t_tbl *tbl = (t_tbl *)arg;
	int i;

	i = -1;
	while (1)
	{
		while (++i < tbl->num_of_philo)
		{
			if (get_current_time() - tbl->phls[i].le > tbl->ttd)
			{
				tbl->philo_died = 1;
				pthread_mutex_lock(&tbl->print);
				printf("%d %d died\n", get_current_dif_time(tbl->start),
					tbl->phls[i].id);
				pthread_mutex_destroy(&tbl->print);
				close_tbl(tbl);
				return (NULL);
			}
		}
		i = -1;
		//ft_usleep(50);
	}
}

void	eating(t_phl *philo)
{
	t_tbl *tbl = philo->tbl;

	take_forks(philo);
	pthread_mutex_lock(&tbl->print);
	printf("%d %d is eating\n", get_current_dif_time(tbl->start), philo->id);
	pthread_mutex_unlock(&tbl->print);
	philo->le = get_current_time();
	ft_usleep(tbl->tte * 1000);
	philo->te++;
	drop_forks(philo);
}

void	sleeping(t_phl *philo)
{
	t_tbl *tbl = philo->tbl;

	pthread_mutex_lock(&tbl->print);
	printf("%d %d is sleeping\n", get_current_dif_time(tbl->start), philo->id);
	pthread_mutex_unlock(&tbl->print);
	ft_usleep(tbl->tts * 1000);
	pthread_mutex_lock(&tbl->print);
	printf("%d %d is thinking\n", get_current_dif_time(tbl->start), philo->id);
	pthread_mutex_unlock(&tbl->print);
	//ft_usleep(50);
}

void	*dinner_routine(void *arg)
{
	t_phl *philo = (t_phl *)arg;
	t_tbl *tbl = philo->tbl;

	//pthread_mutex_lock(&tbl->ready_mtx);
	while (tbl->ready == 0)
		;
		//pthread_mutex_unlock(&tbl->ready_mtx);
	//printf("after being ready philo = %d\n", philo->id);
	if (tbl->num_of_philo == 1)
	{
		ft_usleep(tbl->ttd * 1000);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		ft_usleep(tbl->tte * 1000 / 10);
	while (tbl->philo_died == 0)
	{
		if ((tbl->tme != -1 && philo->te >= tbl->tme))
			break ;
		//printf("before eating philo = %d\n", philo->id);
		eating(philo);
		//printf("after eating philo = %d\n", philo->id);
		sleeping(philo);
	}
	return (NULL);
}

void	close_tbl(t_tbl *tbl)
{
	int i;

	i = -1;
	while (++i < tbl->num_of_philo)
		pthread_mutex_destroy(&tbl->forks[i]);
	while (++i < tbl->num_of_philo)
		pthread_detach(tbl->phls[i].thread);
	pthread_detach(tbl->check_death);
	free(tbl->forks);
	free(tbl->phls);
	free(tbl);
	exit(0);
}

int	main(int argc, char *argv[])
{
	t_tbl *tbl;

	if (argc < 5 || argc > 6)
	{
		instructions();
		exit_error("Error: Invalid number of arguments\n");
	}
	tbl = malloc(sizeof(t_tbl));
	if (!tbl)
		exit_error("Error: Malloc failed\n");
	start_tbl(tbl, argv, argc);
	return (0);
}