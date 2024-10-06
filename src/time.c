#include "../include/philo.h"

long long	time_elapsed(struct timeval start)
{
	struct timeval	current;
	long long	time;

	gettimeofday(&current, NULL);
	time = (current.tv_sec - start.tv_sec) * 1000LL + (current.tv_usec
			- start.tv_usec) / 1000;
	return (time);
}

long long	get_current_time(void)
{
	struct timeval	current;
	long long	time;

	gettimeofday(&current, NULL);
	time = (current.tv_sec * 1000LL) + (current.tv_usec / 1000);
	return (time);
}

void	ft_usleep(long long microseconds)
{
	struct timeval	start;
	struct timeval	current;
	long long	elapsed;

	elapsed = 0;
	gettimeofday(&start, NULL);
	while (elapsed < microseconds)
	{
		usleep(50);
		gettimeofday(&current, NULL);
		elapsed = (current.tv_sec - start.tv_sec) * 1000000LL + (current.tv_usec
				- start.tv_usec);
	}
}