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
	pthread_t		thread;
	int				id; // id of the philosopher
	int				*forks; // array of forks
	int				*is_eating; // array of flags to check if the philosopher is eating
	long			*last_eat; // array of timestamps of the last time the philosopher ate
	int				*is_dead; // flag to check if the philosopher is dead
}	t_philo;

// rules struct
typedef struct s_rules
{
	int				num_of_philo; // number of philosophers
	int				time_to_die; // time to die in milliseconds
	int				time_to_eat; // time to eat in milliseconds
	int				time_to_sleep; // time to sleep in milliseconds
	int				times_must_eat; // number of times each philosopher must eat
	struct timeval			start; // start time
}	t_rules;

// utils.c
long	ft_atol(char *str); // convert string to long
void	instructions(void); // print instructions
void	exit_error(char *str); // print error message and exit

#endif
