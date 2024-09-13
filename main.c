#include"philo.h"

void setting_rules(t_rules *rules, char *argv, int argc) // Hacer chequeo de errores en los argumentos antes de atol
{
	rules->num_of_philo = ft_atol(&argv[1]);
	rules->time_to_die = ft_atol(&argv[2]);
	rules->time_to_eat = ft_atol(&argv[3]);
	rules->time_to_sleep = ft_atol(&argv[4]);
	if(argc == 6)
		rules->times_must_eat = ft_atol(&argv[5]);
}

int	main(int argc, char *argv[])
{
	t_rules *rules;

	if (argc < 5 || argc > 6)
	{
		instructions();
	}
	rules = malloc(sizeof(t_rules));
	if (!rules)
		return (1);
	setting_rules(rules, *argv, argc);
	return (0);
}