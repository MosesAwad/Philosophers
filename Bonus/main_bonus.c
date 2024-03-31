/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 00:27:08 by mawad             #+#    #+#             */
/*   Updated: 2024/03/28 02:48:27 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	wait_philos(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->philo_amnt)
	{
		handle_semaphores(program->wait_sem, NULL, 0, WAIT);
		i++;
	}
	handle_semaphores(program->wait_sem, NULL, 0, WAIT);
	i = 0;
	while (i < program->philo_amnt)
	{
		kill(program->philos[i], SIGINT);
		i++;
	}
	handle_semaphores(program->kill_sem, NULL, 0, POST);
}

int	main(int argc, char *argv[])
{
	t_program	program;

	if (!(argc == 5 || argc == 6))
	{
		printf("Please insert 5 or 6 strict arguments only\n");
		return (1);
	}
	parser(argv);
	init_program(&program, argv);
	set_up_procs(&program);
	wait_philos(&program);
	ft_destroy(&program);
	return (0);
}
