#include "../include/philo.h"

void	close_table(t_table *table);
void	check_if_philo_died(t_philo *philo);

void	setting_table(t_table *table, char **argv, int argc)
{
	int i;

	i = -1;
	gettimeofday(&table->start, NULL);
	table->num_of_philo = ft_atol(argv[1]);
	table->time_to_die = ft_atol(argv[2]);
	table->time_to_eat = ft_atol(argv[3]);
	table->time_to_sleep = ft_atol(argv[4]);
	table->times_must_eat = -1;
	if (argc == 6)
		table->times_must_eat = ft_atol(argv[5]);
	table->philos = malloc(sizeof(t_philo) * table->num_of_philo);
	table->forks = malloc(sizeof(pthread_mutex_t) * table->num_of_philo);
	if (!table->forks || !table->philos)
		exit_error("Error: Malloc failed\n");
	while (++i < table->num_of_philo)
		pthread_mutex_init(&table->forks[i], NULL);
	table->forks_flag = malloc(sizeof(int) * table->num_of_philo);
	if (!table->forks_flag)
		exit_error("Error: Malloc failed\n");
	i = -1;
	while(++i < table->num_of_philo)
		table->forks_flag[i] = 0;
	table->forks_flag[table->num_of_philo] = 0;
	//printf("forks_flag: %d\n", table->forks_flag[table->num_of_philo]);
	pthread_mutex_init(&table->print, NULL);
}

void	create_philos(t_table *table)
{
	int i;

	i = -1;
	while (++i < table->num_of_philo)
	{
		table->philos[i].id = i + 1;
		table->philos[i].l_fork = i;
		table->philos[i].r_fork = (i + 1) % table->num_of_philo;
		table->philos[i].times_eaten = 0;
		table->philos[i].last_eaten = table->start;
		table->philos[i].table = table;
	}
}

int	get_time_diff(struct timeval last_eaten)
{
	struct timeval current;
	int time;

	gettimeofday(&current, NULL);
	time = (current.tv_sec - last_eaten.tv_sec) * 1000 + (current.tv_usec - last_eaten.tv_usec) / 1000;
	return (time);
}

int pseudo_random(int min, int max)
{
    struct timeval time;
    gettimeofday(&time, NULL);

    // Use the microsecond part of the current time to create a pseudo-random number
    long seed = time.tv_usec;

    // Generate a number between min and max using the seed
    return (seed % (max - min + 1)) + min;
}

// Function to calculate a random wait time
int get_random_wait_time(int time_to_die, int time_to_eat, int time_to_sleep)
{
    // Calculate a maximum safe wait time (slightly less than time_to_die)
    int max_safe_time = time_to_die - time_to_eat;
    // Generate a random wait time within reasonable bounds
    int wait_time = pseudo_random(time_to_eat / 2, time_to_sleep + time_to_eat);

    // Ensure the wait time is within a safe limit
    if (wait_time >= max_safe_time)
        wait_time = max_safe_time - pseudo_random(1, 100); // Leave margin to avoid dying

    return wait_time;
}

void	wait_for_fork(t_table *table)
{
	int	tt_wait;

	tt_wait = get_random_wait_time(table->time_to_die, table->time_to_eat, table->time_to_sleep);
	if (tt_wait < 0)
		tt_wait = table->time_to_die;
	usleep(tt_wait * 1000);
}

void	take_forks(t_philo *philo)
{
	t_table *table = philo->table;

	check_if_philo_died(philo);
	if(pthread_mutex_lock(&table->forks[philo->r_fork]) != 0)
		exit_error("Error: Failed to lock right fork\n");
	table->forks_flag[philo->r_fork] = 1;
	pthread_mutex_lock(&table->print);
	printf("%dms %d has taken a fork\n", get_current_time(table->start), philo->id);
	pthread_mutex_unlock(&table->print);
	if(table->forks_flag[philo->l_fork] == 1)
	{
		pthread_mutex_unlock(&table->forks[philo->r_fork]);
		table->forks_flag[philo->r_fork] = 0;
		wait_for_fork(table);
		take_forks(philo);
	}
	else
	{
		if(pthread_mutex_lock(&table->forks[philo->l_fork]) != 0)
			exit_error("Error: Failed to lock left fork\n");
		table->forks_flag[philo->l_fork] = 1;
		pthread_mutex_lock(&table->print);
		printf("%dms %d has taken a fork\n", get_current_time(table->start), philo->id);
		pthread_mutex_unlock(&table->print);
	}
}

void	check_if_philo_died(t_philo *philo)
{
	t_table *table = philo->table;

	if (table->time_to_die < get_time_diff(philo->last_eaten))
	{
		pthread_mutex_lock(&table->print);
		printf("%dms %d died\n", get_current_time(table->start), philo->id);
		pthread_mutex_unlock(&table->print);
		pthread_mutex_unlock(&table->forks[philo->l_fork]);
		table->forks_flag[philo->l_fork] = 0;
		pthread_mutex_unlock(&table->forks[philo->r_fork]);
		table->forks_flag[philo->r_fork] = 0;
		table->philo_died = 1;
		close_table(table);
		exit(0);
	}
}

void	*dinner_routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	t_table *table = philo->table;

	while (1)
	{
		if((table->times_must_eat != -1 && philo->times_eaten >= table->times_must_eat) || table->philo_died)
			break ;
		// Think
		pthread_mutex_lock(&table->print);
		printf("%dms %d is thinking\n", get_current_time(table->start), philo->id);
		pthread_mutex_unlock(&table->print);
		check_if_philo_died(philo);

		// Try to pick up the left and right forks (mutexes)
		take_forks(philo);
		// Eating
		pthread_mutex_lock(&table->print);
		printf("%dms %d is eating\n", get_current_time(table->start),philo->id);
		pthread_mutex_unlock(&table->print);

		// Simulate eating for `time_to_eat` milliseconds
		usleep(table->time_to_eat * 1000);

		// Update the time philosopher last ate
		gettimeofday(&philo->last_eaten, NULL);
		philo->times_eaten++;
		check_if_philo_died(philo);
		// Put down the forks
		pthread_mutex_unlock(&table->forks[philo->r_fork]);
		table->forks_flag[philo->r_fork] = 0;
		pthread_mutex_unlock(&table->forks[philo->l_fork]);
		table->forks_flag[philo->l_fork] = 0;

		// Sleep
		pthread_mutex_lock(&table->print);
		printf("%dms %d is sleeping\n", get_current_time(table->start), philo->id);
		pthread_mutex_unlock(&table->print);

		// Simulate sleeping for `time_to_sleep` milliseconds
		usleep(table->time_to_sleep * 1000);
	}
	return (NULL);
}

void	start_philosophers(t_table *table)
{
	int i;

	i = -1;
	while (++i < table->num_of_philo)
	{
		if (pthread_create(&table->philos[i].thread, NULL, &dinner_routine,
				&table->philos[i]) != 0)
			exit_error("Error: Failed to create philosopher thread\n");
	}
	i = -1;
	// Join the threads after they're done (this keeps the main program waiting)
	while (++i < table->num_of_philo)
	{
		if (pthread_join(table->philos[i].thread, NULL) != 0)
			exit_error("Error: Failed to join philosopher thread\n");
	}
}

void	start_table(t_table *table, char **argv, int argc)
{
	setting_table(table, argv, argc);
	create_philos(table);
	start_philosophers(table);
}

void	close_table(t_table *table)
{
	int i;

	i = -1;
	while (++i < table->num_of_philo)
		pthread_mutex_destroy(&table->forks[i]);
	free(table->forks);
	free(table->philos);
}

int	main(int argc, char *argv[])
{
	t_table *table;

	if (argc < 5 || argc > 6)
	{
		instructions();
		exit_error("Error: Invalid number of arguments\n");
	}
	table = malloc(sizeof(t_table));
	if (!table)
		exit_error("Error: Malloc failed\n");
	start_table(table, argv, argc);
	close_table(table);
	//usleep(1000000);//
	//print_current_time(table->start);//
	return (0);
}