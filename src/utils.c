/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smanriqu <smanriqu@student.42berlin.d      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 11:33:50 by smanriqu          #+#    #+#             */
/*   Updated: 2024/10/08 11:33:52 by smanriqu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

static int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

void	input_error(char *str, t_tbl *tbl)
{
	free(tbl);
	printf("%s", str);
	exit(1);
}

static char	*validate_input(char *str, t_tbl *tbl)
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
		input_error("Error: Argument cannot be negative\n", tbl);
	while (is_digit(str[i]))
		i++;
	if (str[i] != '\0')
		input_error("Error: Argument contains invalid characters\n", tbl);
	return (str);
}

long long	ft_atoll(char *str, t_tbl *tbl)
{
	int			i;
	long long	result;
	char		*input;

	i = 0;
	result = 0;
	input = validate_input(str, tbl);
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
