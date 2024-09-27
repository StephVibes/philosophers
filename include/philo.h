#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

//philosopher struct
typedef struct s_philo
{
	pthread_t		thread; // thread
	int				id; // id
	int				l_fork; // left fork
	int				r_fork; // right fork
	int				times_eaten; // times eaten
	struct timeval	last_eaten; // last eaten time
	struct s_table		*table; // table
}	t_philo;

typedef struct s_table
{
	int				num_of_philo; // number of philosophers
	int				time_to_die; // time to die in milliseconds
	int				time_to_eat; // time to eat in milliseconds
	int				time_to_sleep; // time to sleep in milliseconds
	int				times_must_eat; // number of times each philosopher must eat
	struct timeval			start; // start time
	pthread_mutex_t	*forks; // forks
	pthread_mutex_t	print; // print mutex
	int			current_time; // current time
	int			philo_died; // philo died
	t_philo			*philos; // philosophers
}	t_table;

// utils.c
long	ft_atol(char *str); // convert string to long
void	instructions(void); // print instructions
void	exit_error(char *str); // print error message and exit
int	get_current_time(struct timeval start); // calculate time since start

#endif
