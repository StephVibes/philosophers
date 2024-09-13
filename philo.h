#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>

//philosopher struct
typedef struct s_philo
{
	int				id;
	int				*forks;
	int				*is_eating;
	long			*last_eat;
	int				*is_dead;
}	t_philo;

// rules struct
typedef struct s_rules
{
	int				num_of_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				times_must_eat;
}	t_rules;

// utils.c
long	ft_atol(char *str);
void	instructions(void);

#endif
