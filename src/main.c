#include "../include/philo.h"

void	close_tbl(t_tbl *tbl);
void	check_if_philo_died(t_phl *philo);
void	take_forks(t_phl *philo);

void	drop_forks(t_phl *philo)
{
	t_tbl *tbl = philo->tbl;

	pthread_mutex_unlock(&tbl->forks[philo->rf]);
	tbl->forks_flag[philo->rf] = 0;
	pthread_mutex_unlock(&tbl->forks[philo->lf]);
	tbl->forks_flag[philo->lf] = 0;
}

void	take_forks(t_phl *philo)
{
	t_tbl *tbl = philo->tbl;

	check_if_philo_died(philo);
	if (pthread_mutex_lock(&tbl->forks[philo->rf]) != 0)
		exit_error("Error: Failed to lock right fork\n");
	pthread_mutex_lock(&tbl->print);
	printf("%d %d has taken a fork\n", get_current_dif_time(tbl->start), philo->id);
	pthread_mutex_unlock(&tbl->print);
	if (pthread_mutex_lock(&tbl->forks[philo->lf]) != 0)
		exit_error("Error: Failed to lock left fork\n");
	pthread_mutex_lock(&tbl->print);
	printf("%d %d has taken a fork\n", get_current_dif_time(tbl->start), philo->id);
	pthread_mutex_unlock(&tbl->print);
	check_if_philo_died(philo);
}

void	check_if_philo_died(t_phl *philo)
{
	t_tbl *tbl = philo->tbl;

	if (tbl->ttd < (get_current_time() - philo->le))
	{
		tbl->philo_died = 1;
		pthread_mutex_lock(&tbl->print);
		printf("%d %d died\n", get_current_dif_time(tbl->start), philo->id);
		pthread_mutex_unlock(&tbl->print);
		pthread_mutex_unlock(&tbl->forks[philo->lf]);
		tbl->forks_flag[philo->lf] = 0;
		pthread_mutex_unlock(&tbl->forks[philo->rf]);
		tbl->forks_flag[philo->rf] = 0;
		close_tbl(tbl);
		exit(0);
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
	check_if_philo_died(philo);
}

void	sleeping(t_phl *philo)
{
	t_tbl *tbl = philo->tbl;

	check_if_philo_died(philo);
	pthread_mutex_lock(&tbl->print);
	printf("%d %d is sleeping\n", get_current_dif_time(tbl->start), philo->id);
	pthread_mutex_unlock(&tbl->print);
	ft_usleep(tbl->tts * 1000);
	pthread_mutex_lock(&tbl->print);
	printf("%d %d is thinking\n", get_current_dif_time(tbl->start), philo->id);
	pthread_mutex_unlock(&tbl->print);
	check_if_philo_died(philo);
}

void	*dinner_routine(void *arg)
{
	t_phl *philo = (t_phl *)arg;
	t_tbl *tbl = philo->tbl;

	while (!tbl->ready)
		;
	if(tbl->num_of_philo == 1)
	{
		ft_usleep(tbl->ttd * 1000);
		printf("%d %d died\n", get_current_dif_time(tbl->start), philo->id);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		ft_usleep(100);
	while (tbl->philo_died == 0)
	{
		if ((tbl->tme != -1 && philo->te >= tbl->tme))
			break ;
		eating(philo);
		sleeping(philo);
		//ft_usleep(50);
		check_if_philo_died(philo);
	}
	return (NULL);
}

void	close_tbl(t_tbl *tbl)
{
	int i;

	i = -1;
	while (++i < tbl->num_of_philo)
		pthread_mutex_destroy(&tbl->forks[i]);
	pthread_mutex_destroy(&tbl->print);
	free(tbl->forks_flag);
	free(tbl->forks);
	free(tbl->phls);
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
	close_tbl(tbl);
	return (0);
}