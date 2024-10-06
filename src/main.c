#include "../include/philo.h"

int	main(int argc, char *argv[])
{
	t_tbl	*tbl;

	if (argc < 5 || argc > 6)
	{
		instructions();
		exit_error("Error: Invalid number of arguments\n");
	}
	tbl = malloc(sizeof(t_tbl));
	if (!tbl)
		exit_error("Error: Malloc failed\n");
	setting_tbl(tbl, argv, argc);
	init_philos(tbl);
	start_philosophers(tbl);
	return (0);
}