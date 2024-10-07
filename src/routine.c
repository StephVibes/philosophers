#include "../include/philo.h"

void	*routine(void *arg)
{
	t_phl	*philo;
	t_tbl	*tbl;

	philo = (t_phl *)arg;
	tbl = philo->tbl;
	while (tbl->ready == 0)
		;
	if (tbl->num_of_philo == 1)
	{
		if (pthread_mutex_lock(&tbl->forks[philo->rf]) != 0)
			exit_error("Error: Failed to lock right fork\n");
		print_status(philo, "has taken a fork");
		ft_usleep(tbl->ttd * 1000LL);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		ft_usleep(tbl->tte * 500LL);
	while (tbl->philo_died == 0 && tbl->all_ate == 0)
	{
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
	philo->le = get_current_time();
	ft_usleep(tbl->tte * 1000LL);
	philo->te++;
	pthread_mutex_unlock(&tbl->forks[philo->rf]);
	pthread_mutex_unlock(&tbl->forks[philo->lf]);
}

void	take_forks(t_phl *philo)
{
	t_tbl	*tbl;

	tbl = philo->tbl;
	if (pthread_mutex_lock(&tbl->forks[philo->rf]) != 0)
		exit_error("Error: Failed to lock right fork\n");
	print_status(philo, "has taken a fork");
	if (pthread_mutex_lock(&tbl->forks[philo->lf]) != 0)
		exit_error("Error: Failed to lock left fork\n");
	print_status(philo, "has taken a fork");
}

void	sleeping(t_phl *philo)
{
	t_tbl	*tbl;

	tbl = philo->tbl;
	print_status(philo, "is sleeping");
	ft_usleep(tbl->tts * 1000LL);
	print_status(philo, "is thinking");
}
