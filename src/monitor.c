#include "../include/philo.h"

void	print_status(t_phl *philo, char *status)
{
	t_tbl	*tbl;

	tbl = philo->tbl;
	if (tbl->philo_died || tbl->all_ate)
		return ;
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
			tbl->philo_died = 1;
			pthread_mutex_lock(&tbl->print);
			printf("%lld %d died\n", time_elapsed(tbl->start), tbl->phls[i].id);
			pthread_mutex_unlock(&tbl->print);
			return (1);
		}
	}
	return (0);
}

static int	check_all_philos_ate_enough(t_tbl *tbl)
{
	int i;

	if (tbl->tme == -1)
		return (0);
	i = -1;
	while (++i < tbl->num_of_philo)
	{
		if (tbl->phls[i].te < tbl->tme)
			return (0);
	}
	tbl->all_ate = 1;
	return (1);
}

void	*monitor(void *arg)
{
	t_tbl *tbl;
	tbl = (t_tbl *)arg;

	while (1)
	{
		if (check_philo_death(tbl))
			return (NULL);
		if (check_all_philos_ate_enough(tbl))
			return (NULL);
		ft_usleep(500LL);
	}
}

