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
	pthread_t thread;          // thread
	int id;                    // id
	int lf;                // left fork
	int rf;                // right fork
	int te;               // times eaten
	int le; // last eaten time
	struct s_tbl *tbl;     // table
} t_phl;

typedef struct s_tbl
{
	int num_of_philo;       // number of philosophers
	int ttd;            // time to die in milliseconds
	int tte;            // time to eat in milliseconds
	int tts;          // time to sleep in milliseconds
	int tme;          // number of times each philosopher must eat
	struct timeval start;   // start time
	pthread_mutex_t *forks; // forks
	pthread_mutex_t print;  // print mutex
	int philo_died;         // philo died
	t_phl *phls;        // philosophers
	int ready;             // ready flag
	pthread_mutex_t ready_mtx; // ready mutex
	pthread_t check_death; // check death thread
} t_tbl;

// utils.c
long	ft_atol(char *str);                    // convert string to long
void	instructions(void);                    // print instructions
void	exit_error(char *str);                 // print error message and exit
int	get_current_dif_time(struct timeval start); // calculate time since start
void	ft_usleep(long microseconds);		  // sleep for microseconds

void	*dinner_routine(void *arg);
void	*monitor(void *arg);
void	close_tbl(t_tbl *tbl);

// init.c
void	start_philosophers(t_tbl *tbl); // start philosophers
void	setting_tbl(t_tbl *tbl, char **argv, int argc); // set table
void	create_philos(t_tbl *tbl); // create philosophers
void	start_tbl(t_tbl *tbl, char **argv, int argc); // start table
int	get_current_time(void); // get current time

#endif
