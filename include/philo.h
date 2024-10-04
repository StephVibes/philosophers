#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

// philosopher struct
typedef struct s_phl
{
	pthread_t		thread;
	int				id;
	int				lf;
	int				rf;
	int				te;
	int				le;
	struct s_tbl	*tbl;
}	t_phl;

typedef struct s_tbl
{
	int				num_of_philo;
	int				ttd;
	int				tte;
	int				tts;
	int				tme;
	struct timeval	start;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	int				philo_died;
	t_phl			*phls;
	int				ready;
	pthread_mutex_t	ready_mtx;
	pthread_t		check_death;
}	t_tbl;

// utils.c
long	ft_atol(char *str);
void	instructions(void);
void	exit_error(char *str);
int		get_current_dif_time(struct timeval start);
void	ft_usleep(long microseconds);

void	*dinner_routine(void *arg);
void	*monitor(void *arg);
void	close_tbl(t_tbl *tbl);

// init.c
void	start_philosophers(t_tbl *tbl);
void	setting_tbl(t_tbl *tbl, char **argv, int argc);
void	create_philos(t_tbl *tbl);
void	start_tbl(t_tbl *tbl, char **argv, int argc);
int		get_current_time(void);

#endif
