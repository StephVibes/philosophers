#include "../include/philo.h"

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
	pthread_mutex_init(&table->print, NULL); //?
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

void	*dinner_routine(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	t_table *table = philo->table;

	//printf("Philosopher %d is alive\n", philo->id);
	while (1)
	{
		if((table->times_must_eat != -1 && philo->times_eaten >= table->times_must_eat) || table->philo_died)
			break ;
		// Think
		pthread_mutex_lock(&table->print);
		printf("%dms %d is thinking\n", get_current_time(table->start), philo->id);
		pthread_mutex_unlock(&table->print);
		if (table->time_to_die < get_time_diff(philo->last_eaten))
		{
			pthread_mutex_lock(&table->print);
			printf("%dms %d died\n", get_current_time(table->start) ,philo->id);
			pthread_mutex_unlock(&table->print);
			pthread_mutex_unlock(&table->forks[philo->l_fork]);
			pthread_mutex_unlock(&table->forks[philo->r_fork]);
			table->philo_died = 1;
			break ;
		}
		// Try to pick up the left and right forks (mutexes)
		pthread_mutex_lock(&table->forks[philo->l_fork]);
		pthread_mutex_lock(&table->forks[philo->r_fork]);

		// Check if the philosopher has died

		// Eating
		pthread_mutex_lock(&table->print);
		printf("%dms %d is eating\n", get_current_time(table->start),philo->id);
		pthread_mutex_unlock(&table->print);

		// Simulate eating for `time_to_eat` milliseconds
		usleep(table->time_to_eat * 1000);

		// Update the time philosopher last ate
		gettimeofday(&philo->last_eaten, NULL);
		philo->times_eaten++;

		// Put down the forks
		pthread_mutex_unlock(&table->forks[philo->r_fork]);
		pthread_mutex_unlock(&table->forks[philo->l_fork]);

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
		//printf("Creating philosopher %d\n", i + 1);
		// Create a new thread for each philosopher
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
		//printf("Philosopher %d has finished eating\n", i + 1);
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