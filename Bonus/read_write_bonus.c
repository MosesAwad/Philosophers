/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_write_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 02:44:46 by mawad             #+#    #+#             */
/*   Updated: 2024/03/28 03:23:50 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_bool	game_over(t_program *program)
{
	t_bool	result;

	handle_semaphores(program->philo_data.end_sem, NULL, 0, WAIT);
	if (program->game_over == FALSE)
		result = FALSE;
	else
		result = TRUE;
	handle_semaphores(program->philo_data.end_sem, NULL, 0, POST);
	return (result);
}

void	bool_writer(sem_t *semaphore, t_bool *dest, t_bool new_value)
{
	(void) semaphore;
	handle_semaphores(semaphore, NULL, 0, WAIT);
	*dest = new_value;
	handle_semaphores(semaphore, NULL, 0, POST);
}
