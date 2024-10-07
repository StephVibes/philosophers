#include "../include/philo.h"

static int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
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

long long	ft_atoll(char *str)
{
	int			i;
	long long	result;
	char		*input;

	i = 0;
	result = 0;
	input = validate_input(str);
	while (input[i] >= '0' && input[i] <= '9')
	{
		result = result * 10 + (input[i] - '0');
		i++;
	}
	if (result > 9223372036854775807LL)
		exit_error("Error: Argument out of range\n");
	return (result);
}

void	exit_error(char *str)
{
	printf("%s", str);
	exit(1);
}
