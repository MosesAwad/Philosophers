/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 00:25:30 by mawad             #+#    #+#             */
/*   Updated: 2024/03/26 00:26:43 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_forks(t_fork *forks, int phil_amnt)
{
	int	i;

	i = 0;
	while (i < phil_amnt)
	{
		forks[i].fork_id = i;
		handle_mutexes(&(forks[i].fork), INIT);
		i++;
	}
}

void	init_philos(t_program *program)
{
	int	i;
	int	n;

	i = 0;
	n = program->philo_amnt;
	while (i < n)
	{
		program->philos[i].philo_id = i + 1;
		program->philos[i].left_fork = &(program->forks[(i + 1) % n]);
		program->philos[i].right_fork = &(program->forks[i]);
		program->philos[i].meal_count = 0;
		handle_mutexes(&(program->philos[i].philo_mutex), INIT);
		program->philos[i].philo_full = FALSE;
		program->philos[i].program = program;
		program->philos[i].last_meal_time = 0;
		i++;
	}
}

//The reason why I am multiplying the times by 1e3, aka 1000 is
//because my ft_usleep (a more accurate rendition) of the original
//usleep takes as its argument MICROseconds, hence the 'u' in usleep.
//Thus, since the subject requires us to input the time arguments in
//units of milliseconds, we have to convert them here to microseconds
//such that my ft_usleep can work appropriately.
//I initialize meal_limit to -1 if there is no argv[5] because if I
//initiliaze it to 0, then in that case my check_full function would
//not work because meal_count would be 0 and meal_limit would be 0,
//since they are equal, it would say that the philosopher is full before
//we ever run the simulation. So easiest way to bypass this issue is just
//to set meal_limit to -1 if the user did not specify a meal limit.
void	init_program(t_program *program, char *argv[])
{
	program->philo_amnt = ft_atoi(argv[1]);
	program->time_to_die = ft_atoi(argv[2]) * 1e3;
	program->time_to_eat = ft_atoi(argv[3]) * 1e3;
	program->time_to_sleep = ft_atoi(argv[4]) * 1e3;
	if (argv[5])
		program->meal_limit = ft_atoi(argv[5]);
	else
		program->meal_limit = -1;
	program->forks = ft_alloc(sizeof(t_fork) * program->philo_amnt);
	program->game_over = FALSE;
	init_forks(program->forks, program->philo_amnt);
	program->philos = ft_alloc(sizeof(t_philo) * program->philo_amnt);
	init_philos(program);
	handle_mutexes(&(program->read_mutex), INIT);
	handle_mutexes(&(program->write_mutex), INIT);
}
