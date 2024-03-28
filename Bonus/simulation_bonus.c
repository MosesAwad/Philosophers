/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 23:57:12 by mawad             #+#    #+#             */
/*   Updated: 2024/03/28 03:23:55 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static char	*get_sem_name(t_program *program)
{
	char	*prefix;
	char	*number;
	char	*sem_name;

	prefix = ft_strdup("readsem");
	number = ft_itoa(program->philo_data.philo_id);
	sem_name = ft_strjoin(prefix, number);
	free(number);
	return (sem_name);
}

static void	dinner_simulation(t_program *program)
{
	char	*sem_name;

	// if (program->philo_data.philo_id % 2 == 0)
	// 	ft_usleep(1000, program);
	sem_name = get_sem_name(program);
	handle_semaphores(NULL, sem_name, 0, UNLINK);
	program->philo_data.read_sem = handle_semaphores(NULL, sem_name,
			1, OPEN);
	free(sem_name);
	handle_semaphores(NULL, "end_sem", 0, UNLINK);
	program->philo_data.end_sem = handle_semaphores(NULL, "end_sem",
			1, OPEN);
	handle_threads(&(program->philo_data.monitor),
		monitor_simulation, program, CREATE);
	while (game_over(program) == FALSE)
	{
		ft_eat(program);
		ft_sleep(program);
		ft_think(program);
	}
	handle_threads(&(program->philo_data.monitor), NULL, NULL, JOIN);
	handle_semaphores(program->philo_data.read_sem, NULL, 0, CLOSE);
	handle_semaphores(program->philo_data.end_sem, NULL, 0, CLOSE);
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
	i = 0;
	while (i < program->philo_amnt)
		waitpid(program->philos[i++], NULL, 0);
}
