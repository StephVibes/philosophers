#include "../include/philo.h"

void	ft_usleep(long microseconds)
{
	struct timeval	start;
	struct timeval	current;
	long			elapsed;

	elapsed = 0;
	gettimeofday(&start, NULL);
	while (elapsed < microseconds)
	{
		usleep(50);
		gettimeofday(&current, NULL);
		elapsed = (current.tv_sec - start.tv_sec) * 1000000 + (current.tv_usec
				- start.tv_usec);
	}
}

void	ft_putnbr_fd(int n, int fd)
{
	char	c;

	if (n < 0)
	{
		write(fd, "-", 1);
		n = -n;
	}
	if (n > 9)
		ft_putnbr_fd(n / 10, fd);
	c = n % 10 + '0';
	write(fd, &c, 1);
}

int	get_current_time(void)
{
	struct timeval	current;
	int				time;

	gettimeofday(&current, NULL);
	time = (current.tv_sec) * 1000 + (current.tv_usec) / 1000;
	return (time);
}

int	get_current_dif_time(struct timeval start)
{
	struct timeval	current;
	int				time;

	gettimeofday(&current, NULL);
	time = (current.tv_sec - start.tv_sec) * 1000 + (current.tv_usec
			- start.tv_usec) / 1000;
	return (time);
}

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

void	exit_error(char *str)
{
	printf("%s", str);
	exit(1);
}

static char	*validate_input(char *str)
{
	size_t	i;

	i = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
	{
		str++;
		i++;
	}
	if (*str == '+')
	{
		str++;
		i++;
	}
	else if (*str == '-')
		exit_error("Error: Negative number\n");
	while (is_digit(str[i]))
		i++;
	if (str[i] != '\0')
		exit_error("Error: Invalid argument\n");
	return (str);
}

long	ft_atol(char *str)
{
	int		i;
	long	result;
	char	*input;

	i = 0;
	result = 0;
	input = validate_input(str);
	while (input[i] >= '0' && input[i] <= '9')
	{
		result = result * 10 + input[i] - '0';
		i++;
	}
	if (result > 2147483647)
		exit_error("Error: Argument out of range\n");
	return (result);
}

void	instructions(void)
{
	printf("**************************************\n");
	printf("**Instructions**\n");
	printf("**Example:**\n");
	printf("  ./philo 5 800 200 200 7\n");
	printf("  This starts the simulation with 5 philosophers,");
	printf("where each can survive 800 ms without eating,\n");
	printf("  takes 200 ms to eat, sleeps for 200 ms,");
	printf("and each philosopher must eat 7 times.\n\n");
	printf("****************************************\n\n");
}