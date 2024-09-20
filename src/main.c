#include "../include/philo.h"

void setting_rules(t_rules *rules, char **argv, int argc) // Hacer chequeo de errores en los argumentos antes de atol
{
	gettimeofday(&rules->start, NULL);
	rules->num_of_philo = ft_atol(argv[1]);
	rules->time_to_die = ft_atol(argv[2]);
	rules->time_to_eat = ft_atol(argv[3]);
	rules->time_to_sleep = ft_atol(argv[4]);
	if(argc == 6)
		rules->times_must_eat = ft_atol(argv[5]);
}

int	main(int argc, char *argv[])
{
	t_rules *rules;

	if (argc < 5 || argc > 6)
	{
		instructions();
		exit_error("Error: Invalid number of arguments\n");
	}
	rules = malloc(sizeof(t_rules));
	if (!rules)
		exit_error("Error: Malloc failed\n");
	setting_rules(rules, argv, argc);
	usleep(1000000);//
	print_current_time(rules->start);//
	return (0);
}