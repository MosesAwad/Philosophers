/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_write_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/28 02:44:46 by mawad             #+#    #+#             */
/*   Updated: 2024/04/02 23:53:18 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

t_bool	check_full(t_program *program)
{
	t_bool	result;

	result = FALSE;
	handle_semaphores(program->philo_data.philo_sem, NULL, 0, WAIT);
	if (program->meal_limit != -1
		&& (program->philo_data.meal_count >= program->meal_limit))
		result = TRUE;
	handle_semaphores(program->philo_data.philo_sem, NULL, 0, POST);
	return (result);
}

void	bool_writer(sem_t *semaphore, t_bool *dest, t_bool new_value)
{
	(void) semaphore;
	handle_semaphores(semaphore, NULL, 0, WAIT);
	*dest = new_value;
	handle_semaphores(semaphore, NULL, 0, POST);
}

void	size_t_writer(sem_t *semaphore, size_t *dest, size_t new_value)
{
	(void) semaphore;
	handle_semaphores(semaphore, NULL, 0, WAIT);
	*dest = new_value;
	handle_semaphores(semaphore, NULL, 0, POST);
}
