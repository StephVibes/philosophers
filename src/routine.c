#include "../include/philo.h"

void	*routine(void *arg)
{
	t_phl	*philo;
	t_tbl	*tbl;

	philo = (t_phl *)arg;
	tbl = philo->tbl;
	while (tbl->ready == 0)
		;
	if (philo->id % 2 == 0)
		ft_usleep(tbl->tte * 500LL);
	while (tbl->philo_died == 0 && (tbl->tme == -1 || philo->te < tbl->tme))
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
	pthread_mutex_lock(&tbl->print);
	printf("%lld %d is eating\n", time_elapsed(tbl->start), philo->id);
	pthread_mutex_unlock(&tbl->print);
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
	pthread_mutex_lock(&tbl->print);
	printf("%lld %d has taken a fork\n", time_elapsed(tbl->start),
		philo->id);
	pthread_mutex_unlock(&tbl->print);
	if (tbl->num_of_philo == 1)
		ft_usleep(tbl->ttd * 1000LL);
	if (pthread_mutex_lock(&tbl->forks[philo->lf]) != 0)
		exit_error("Error: Failed to lock left fork\n");
	pthread_mutex_lock(&tbl->print);
	printf("%lld %d has taken a fork\n", time_elapsed(tbl->start),
		philo->id);
	pthread_mutex_unlock(&tbl->print);
}

void	sleeping(t_phl *philo)
{
	t_tbl	*tbl;

	tbl = philo->tbl;
	pthread_mutex_lock(&tbl->print);
	printf("%lld %d is sleeping\n", time_elapsed(tbl->start), philo->id);
	pthread_mutex_unlock(&tbl->print);
	ft_usleep(tbl->tts * 1000LL);
	pthread_mutex_lock(&tbl->print);
	printf("%lld %d is thinking\n", time_elapsed(tbl->start), philo->id);
	pthread_mutex_unlock(&tbl->print);
}

