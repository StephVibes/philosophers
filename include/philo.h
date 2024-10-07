#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_phl
{
	pthread_t		thread;
	int				id;
	int				lf;
	int				rf;
	int				te;
	long long				le;
	struct s_tbl	*tbl;
}	t_phl;

typedef struct s_tbl
{
	int				num_of_philo;
	int				philo_died;
	int				all_ate;
	int				ready;
	int				tme;
	long long				ttd;
	long long				tte;
	long long				tts;
	struct timeval	start;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print;
	pthread_t		monitor_thr;
	t_phl			*phls;
}	t_tbl;

// utils.c
long long	ft_atoll(char *str);
void	instructions(void);
void	exit_error(char *str);

//time.c
long long		time_elapsed(struct timeval start);
void	ft_usleep(long long microseconds);
long long		get_current_time(void);

//routine.c
void	*routine(void *arg);
void	sleeping(t_phl *philo);
void	take_forks(t_phl *philo);;
void	eating(t_phl *philo);


//monitor.c
void	*monitor(void *arg);
void	close_tbl(t_tbl *tbl);

// init.c
void	init_philos(t_tbl *tbl);
void	start_philosophers(t_tbl *tbl);
void	setting_tbl(t_tbl *tbl, char **argv, int argc);
void	create_philos(t_tbl *tbl);
void	destroy_mutex(t_tbl *tbl);

#endif
