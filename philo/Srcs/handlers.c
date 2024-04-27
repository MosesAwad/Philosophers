/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/12 23:11:01 by mawad             #+#    #+#             */
/*   Updated: 2024/03/26 00:47:52 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	check_thread_error(int status, t_thread_ops operation)
{
	if (status == EAGAIN && operation == CREATE)
		exit_err("The system lacked the necessary resources to create another"
			"thread, or the system-imposed limit on the total number of threads"
			"in a process [PTHREAD_THREADS_MAX] would be exceeded.");
	else if (status == EINVAL && operation == CREATE)
		exit_err("The value specified by attr is invalid.");
	else if (status == EPERM && operation == CREATE)
		exit_err("The caller does not have appropriate permission to set the"
			"required scheduling parameters or scheduling policy.");
	else if (status == EDEADLK && operation == JOIN)
		exit_err("A deadlock was detected or the value of thread specifies"
			"the calling thread.");
	else if (status == EINVAL && operation == JOIN)
		exit_err("The implementation has detected that the value specified"
			"by thread does not refer to a joinable thread.");
	else if (status == ESRCH && operation == JOIN)
		exit_err("No thread could be found corresponding to that specified"
			"by the given thread ID, thread.");
}

void	handle_threads(pthread_t *thread, void *(*routine)(void *),
			void *arg, t_thread_ops operation)
{
	if (operation == CREATE)
		check_thread_error(pthread_create(thread, NULL,
				routine, arg), operation);
	else if (operation == JOIN)
		check_thread_error(pthread_join(*thread, NULL), operation);
}

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
