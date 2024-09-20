#include "../include/philo.h"

size_t	ft_strlen(const char *str)
{
	size_t i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

static int	is_digit(char c)
{
	//printf("c = %c\n", c);
	if (c >= '0' && c <= '9')
	{
		//printf("it's a digit\n");
		return (1);
	}
	//printf("it's not a digit\n");
	return (0);
}

void	exit_error(char *str)
{
	printf("%s", str);
	exit(1);
}

static char *validate_input(char *str)
{
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(str);
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
		exit_error("Error: Negative number\n");
	while(is_digit(str[i]))
		i++;
	if (i != len)
		exit_error("Error: Invalid argument\n");
	return (&str[i]);
}

long	ft_atol(char *str)
{
	int i;
	long result;

	i = 0;
	result = 0;
	str = validate_input(str);
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i] - '0';
		i++;
	}
	if(result > 2147483647)
		exit_error("Error: Argument out of range\n");
	return (result);
}

void	instructions(void)
{
	printf("**************************************************\n");
printf("** Please run:\n ./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat] **\n");
printf("**************************************************\n\n");

printf("**Description of Parameters:**\n");
printf("<number_of_philosophers>\n");
printf("   Number of philosophers at the table.\n\n");

printf("<time_to_die>\n");
printf("   Max time (in milliseconds) a philosopher can go without eating before dying.\n\n");

printf("<time_to_eat>\n");
printf("   Time (in milliseconds) it takes for a philosopher to eat.\n\n");

printf("<time_to_sleep>\n");
printf("   Time (in milliseconds) a philosopher spends sleeping.\n\n");

printf("[number_of_times_each_philosopher_must_eat]\n");
printf("   (Optional) Times each philosopher must eat before ending the simulation.\n\n");

printf("**Example:**\n");
printf("  ./philo 5 800 200 200 7\n");
printf("  This starts the simulation with 5 philosophers, where each can survive 800 ms without eating,\n");
printf("  takes 200 ms to eat, sleeps for 200 ms, and each philosopher must eat 7 times.\n\n");
printf("**************************************************\n\n");
}