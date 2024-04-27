/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 23:57:12 by mawad             #+#    #+#             */
/*   Updated: 2024/04/14 19:02:45 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static char	*get_sem_name1(t_program *program)
{
	char	*prefix;
	char	*number;
	char	*sem_name;

	prefix = ft_strdup("philo_sem");
	number = ft_itoa(program->philo_data.philo_id);
	sem_name = ft_strjoin(prefix, number);
	free(number);
	return (sem_name);
}

static void	solo_philo(t_program *program)
{
	size_t	elapsed;

	write_status(program, FIRST_FORK);
	handle_semaphores(program->forks, NULL, 0, WAIT);
	program->philo_data.last_meal_time = get_time();
	while (get_time() - program->philo_data.last_meal_time
		< (program->time_to_die / 1e3))
		;
	elapsed = get_time() - program->start_time;
	printf("%lu""	%d died\n", elapsed,
		program->philo_data.philo_id);
	handle_semaphores(program->wait_sem, NULL, 0, POST);
	ft_destroy(program);
	exit(0);
}

static void	dinner_simulation(t_program *program)
{
	char	*sem_name;

	if (program->philo_amnt == 1)
		solo_philo(program);
	sem_name = get_sem_name1(program);
	handle_semaphores(NULL, sem_name, 0, UNLINK);
	program->philo_data.philo_sem = handle_semaphores(NULL, sem_name,
			1, OPEN);
	free(sem_name);
	handle_threads(&(program->philo_data.monitor),
		monitor_simulation, program, CREATE);
	while (1)
	{
		if (check_full(program) == TRUE)
			break ;
		ft_eat(program);
		ft_sleep(program);
		ft_think(program);
	}
	handle_threads(&(program->philo_data.monitor), NULL, NULL, JOIN);
	handle_semaphores(program->philo_data.philo_sem, NULL, 0, CLOSE);
	ft_destroy(program);
	exit(0);
}

void	set_up_procs(t_program *program)
{
	int	i;

	i = 0;
	program->start_time = get_time();
	while (i < program->philo_amnt)
	{
		program->philos[i] = fork();
		if (program->philos[i] == -1)
		{
			printf("Fork error\n");
			ft_destroy(program);
			exit(2);
		}
		if (program->philos[i] == 0)
		{
			program->philo_data.philo_id = i + 1;
			dinner_simulation(program);
		}
		i++;
	}
}
