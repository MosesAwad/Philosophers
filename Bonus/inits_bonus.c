/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inits_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 00:37:20 by mawad             #+#    #+#             */
/*   Updated: 2024/04/01 00:17:04 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

//1. Explanation for why balance_sem's value is philo_amnt / 2 is
//explained in the note above ft_eat in the file actions_bonus.c
//2. Explanation for the use of the semaphore called kill_sem is
//explained in the note above philo_survives in the file monitor_bonus.c
static void	init_semaphores(t_program *program)
{
	handle_semaphores(NULL, "forks", 0, UNLINK);
	program->forks = handle_semaphores(NULL, "forks",
			program->philo_amnt, OPEN);
	handle_semaphores(NULL, "wait_sem", 0, UNLINK);
	program->wait_sem = handle_semaphores(NULL, "wait_sem",
			program->philo_amnt, OPEN);
	handle_semaphores(NULL, "write", 0, UNLINK);
	program->write_sem = handle_semaphores(NULL, "write",
			1, OPEN);
	handle_semaphores(NULL, "kill_sem", 0, UNLINK);
	program->kill_sem = handle_semaphores(NULL, "kill_sem",
			1, OPEN);
	handle_semaphores(NULL, "all_procs_ready_sem", 0, UNLINK);
	program->all_procs_ready_sem = handle_semaphores(NULL,
			"all_procs_ready_sem", 1, OPEN);
	handle_semaphores(NULL, "balance_sem", 0, UNLINK);
	program->balance_sem = handle_semaphores(NULL, "balance_sem",
			((program->philo_amnt) / 2), OPEN);
}

static void	init_philo(t_program *program)
{
	program->philo_data.philo_id = 0;
	program->philo_data.meal_count = 0;
	program->philo_data.philo_full = FALSE;
	program->philo_data.program = program;
	program->philo_data.last_meal_time = 0;
}

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
	program->game_over = FALSE;
	init_semaphores(program);
	program->philos = ft_alloc(sizeof(pid_t) * program->philo_amnt);
	init_philo(program);
}
