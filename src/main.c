#include "../include/philo.h"


void	close_tbl(t_tbl *tbl);
void	check_if_philo_died(t_phl *philo);
void	take_forks(t_phl *philo);

int	get_time_diff(struct timeval le)
{
	struct	timeval current;
	int	time;

	gettimeofday(&current, NULL);
	time = (current.tv_sec - le.tv_sec) * 1000 + (current.tv_usec
			- le.tv_usec) / 1000;
	return (time);
}

int	pseudo_random(int min, int max)
{
	struct	timeval time;
	long	seed;

	gettimeofday(&time, NULL);
	seed = time.tv_usec;
	return ((seed % (max - min + 1)) + min);
}

int	get_random_wait_time(int ttd, int tte, int tts)
{
	int max_safe_time = ttd - tte;
	int wait_time = pseudo_random(tte / 2, tts + tte);
	if (wait_time >= max_safe_time)
		wait_time = max_safe_time - pseudo_random(1, 100);
	return (wait_time);
}

int	get_dynamic_wait_time(int ttd, int tte, int tts, struct timeval le)
{
	struct timeval current_time;
	int time_since_last_meal;
	int max_wait_time, wait_time;

	gettimeofday(&current_time, NULL);
	time_since_last_meal = (current_time.tv_sec - le.tv_sec) * 1000
		+ (current_time.tv_usec - le.tv_usec) / 1000;

	int time_left_to_die = ttd - time_since_last_meal;

	max_wait_time = time_left_to_die / 3;
	if (max_wait_time > tte / 2)
		max_wait_time = tte / 2;

	if (max_wait_time <= 0)
		max_wait_time = ttd / 10;
	wait_time = (pseudo_random(1, 100) % max_wait_time) + (tts / 4);
	if (wait_time > time_left_to_die)
		wait_time = time_left_to_die / 2;
	return (wait_time);
}

/*void	wait_forf(t_phl *philo)
{
	int	tt_wait;
	t_tbl *tbl = philo->tbl;

	tt_wait = get_random_wait_time(tbl->ttd, tbl->tte,
			tbl->tts);
	if (tt_wait < 0)
	{
		printf("Error: Invalid wait time\n");
		tt_wait = tbl->ttd/100;
	}
	ft_usleep(tt_wait * 1000);
}*/

void	wait_forf(t_phl *philo)
{
	int tt_wait;
	t_tbl *tbl = philo->tbl;

	tt_wait = get_dynamic_wait_time(tbl->ttd, tbl->tte,
			tbl->tts, philo->le);
	if (tt_wait < 0)
		tt_wait = tbl->ttd / 1000; // Default safe wait time
	// Sleep for calculated wait time
	ft_usleep(tt_wait * 1000);
	check_if_philo_died(philo);
}

void	drop_forks(t_phl *philo)
{
	t_tbl *tbl = philo->tbl;

	pthread_mutex_unlock(&tbl->forks[philo->rf]);
	tbl->forks_flag[philo->rf] = 0;
	pthread_mutex_unlock(&tbl->forks[philo->lf]);
	tbl->forks_flag[philo->lf] = 0;
}

/*void	take_forks(t_phl *philo)
{
	t_tbl *tbl = philo->tbl;

	check_if_philo_died(philo);
	if (pthread_mutex_lock(&tbl->forks[philo->rf]) != 0)
		exit_error("Error: Failed to lock right fork\n");
	tbl->forks_flag[philo->rf] = 1;
	pthread_mutex_lock(&tbl->print);
	printf("%dms %d has taken a fork\n", get_current_time(tbl->start), philo->id);
	pthread_mutex_unlock(&tbl->print);
	if (tbl->forks_flag[philo->lf] == 1)
	{
		pthread_mutex_unlock(&tbl->forks[philo->rf]);
		tbl->forks_flag[philo->rf] = 0;
		wait_forf(philo);
		take_forks(philo);
	}
	else
	{
		if (pthread_mutex_lock(&tbl->forks[philo->lf]) != 0)
			exit_error("Error: Failed to lock left fork\n");
		tbl->forks_flag[philo->lf] = 1;
		pthread_mutex_lock(&tbl->print);
		printf("%dms %d has taken a fork\n", get_current_time(tbl->start),
			philo->id);
		pthread_mutex_unlock(&tbl->print);
	}
}*/

void	philo_fork(t_phl *philo, int ff, int sf)
{
	t_tbl *tbl = philo->tbl;

	check_if_philo_died(philo);
	if (pthread_mutex_lock(&tbl->forks[ff]) != 0)
		exit_error("Error: Failed to lock right fork\n");
	pthread_mutex_lock(&tbl->print);
	printf("%dms %d has taken a fork\n", get_current_time(tbl->start), philo->id);
	pthread_mutex_unlock(&tbl->print);
	if (pthread_mutex_lock(&tbl->forks[sf]) != 0)
		exit_error("Error: Failed to lock left fork\n");
	pthread_mutex_lock(&tbl->print);
	printf("%dms %d has taken a fork\n", get_current_time(tbl->start), philo->id);
	pthread_mutex_unlock(&tbl->print);
}


/*void	philo_fork(t_phl *philo, int ff, int sf)
{
	t_tbl *tbl = philo->tbl;

	if (pthread_mutex_lock(&tbl->forks[ff]) != 0)
		exit_error("Error: Failed to lock right fork\n");
	tbl->forks_flag[ff] = 1;
	pthread_mutex_lock(&tbl->print);
	printf("%dms %d has taken a fork\n", get_current_time(tbl->start), philo->id);
	pthread_mutex_unlock(&tbl->print);
	if (tbl->forks_flag[sf] == 1)
	{
		pthread_mutex_unlock(&tbl->forks[ff]);
		tbl->forks_flag[ff] = 0;
		wait_forf(philo);
		take_forks(philo);
	}
	else
	{
		if (pthread_mutex_lock(&tbl->forks[sf]) != 0)
			exit_error("Error: Failed to lock left fork\n");
		tbl->forks_flag[sf] = 1;
		pthread_mutex_lock(&tbl->print);
		printf("%dms %d has taken a fork\n", get_current_time(tbl->start), philo->id);
		pthread_mutex_unlock(&tbl->print);
	}
}*/

void	take_forks(t_phl *philo)
{
	// if (philo->id % 2 == 0)
		philo_fork(philo, philo->rf, philo->lf);
	// else
	// {
	// 	philo_fork(philo, philo->lf, philo->rf);
	// }
	check_if_philo_died(philo);
}

void	check_if_philo_died(t_phl *philo)
{
	t_tbl *tbl = philo->tbl;

	if (tbl->ttd < get_time_diff(philo->le))
	{
		tbl->philo_died = 1;
		pthread_mutex_lock(&tbl->print);
		printf("%dms %d died\n", get_current_time(tbl->start), philo->id);
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
	printf("%dms %d is eating\n", get_current_time(tbl->start), philo->id);
	pthread_mutex_unlock(&tbl->print);
	gettimeofday(&philo->le, NULL);
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
	printf("%dms %d is sleeping\n", get_current_time(tbl->start), philo->id);
	pthread_mutex_unlock(&tbl->print);
	ft_usleep(tbl->tts * 1000);
	pthread_mutex_lock(&tbl->print);
	printf("%dms %d is thinking\n", get_current_time(tbl->start), philo->id);
	pthread_mutex_unlock(&tbl->print);
}

void	*dinner_routine(void *arg)
{
	t_phl *philo = (t_phl *)arg;
	t_tbl *tbl = philo->tbl;

	if(tbl->num_of_philo == 1)
	{
		ft_usleep(tbl->ttd * 1000);
		printf("%dms %d died\n", get_current_time(tbl->start), philo->id);
		return (NULL);
	}
	if (philo->id % 2 == 0)
		ft_usleep(200);
	while (tbl->philo_died == 0)
	{
		if ((tbl->tme != -1 && philo->te >= tbl->tme))
			break ;
		eating(philo);
		sleeping(philo);
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