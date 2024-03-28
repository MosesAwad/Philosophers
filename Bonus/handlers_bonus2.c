/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers_bonus2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 02:33:02 by mawad             #+#    #+#             */
/*   Updated: 2024/03/27 02:33:40 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	check_mutex_error(int status, t_mutex_ops operation)
{
	if (status == EINVAL && operation == INIT)
		exit_err("The value specified by attr is invalid.");
	else if (status == ENOMEM && operation == INIT)
		exit_err("The process cannot allocate enough memory to"
			"create another mutex.");
	else if (status == EINVAL && (operation == LOCK || operation == UNLOCK))
		exit_err("The value specified by mutex is invalid.");
	else if (status == EDEADLK && operation == LOCK)
		exit_err("A deadlock would occur if the thread blocked"
			"waiting for mutex.");
	else if (status == EPERM && operation == UNLOCK)
		exit_err("The current thread does not hold a lock on mutex.");
	else if (status == EINVAL && operation == DESTROY)
		exit_err("The value specified by mutex is invalid.");
	else if (status == EBUSY && operation == DESTROY)
		exit_err(" Mutex is locked.");
}

void	handle_mutexes(pthread_mutex_t *mutex, t_mutex_ops operation)
{
	if (operation == INIT)
		check_mutex_error(pthread_mutex_init(mutex, NULL), operation);
	else if (operation == LOCK)
		check_mutex_error(pthread_mutex_lock(mutex), operation);
	else if (operation == UNLOCK)
		check_mutex_error(pthread_mutex_unlock(mutex), operation);
	else if (operation == DESTROY)
		check_mutex_error(pthread_mutex_destroy(mutex), operation);
}
