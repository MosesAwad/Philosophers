/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlers_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 00:42:42 by mawad             #+#    #+#             */
/*   Updated: 2024/03/28 00:47:28 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

static void	check_sem_open_error(void)
{
	if (errno == EACCES)
		exit_err("The required permissions (for reading and/or writing) are "
			"denied for the given flags; or O_CREAT is specified, the object "
			"does not exist, and permission to create the semaphoreis denied.");
	else if (errno == EEXIST)
		exit_err("O_CREAT and O_EXCL were specified and the semaphore exists.");
	else if (errno == EINTR)
		exit_err("The sem_open() operation was interrupted by a signal.");
	else if (errno == EINVAL)
		exit_err("The shm_open() operation is not supported; or O_CREAT is "
			"specified and value exceeds SEM_VALUE_MAX.");
	else if (errno == EMFILE)
		exit_err("The process has already reached its limit for semaphores or "
			"file descriptors in use.");
	else if (errno == ENAMETOOLONG)
		exit_err("name exceeded PSEMNAMLEN characters.");
	else if (errno == ENFILE)
		exit_err("Too many semaphores or fd's are open on the system.");
	else if (errno == ENOENT)
		exit_err("O_CREAT is not set and the named semaphore does not exist.");
	else if (errno == ENOSPC)
		exit_err("O_CREAT is specified, the file does not exist, and there is "
			"insufficient space available to create the semaphore.");
}

static void	check_sem_error(int status, t_sem_ops operation)
{
	if (status == EINVAL && (operation == CLOSE || operation == POST
			|| operation == WAIT))
		exit_err("sem is not a valid semaphore descriptor.");
	else if (status == EAGAIN && operation == WAIT)
		exit_err("The semaphore is already locked.");
	else if (status == EDEADLK && operation == WAIT)
		exit_err("A deadlock was detected.");
	else if (status == EINTR && operation == WAIT)
		exit_err("The call was interrupted by a signal.");
	else if (status == EACCES && operation == UNLINK)
		exit_err("Permission is denied to be remove the semaphore.");
	else if (status == ENAMETOOLONG && operation == UNLINK)
		exit_err("name exceeded PSEMNAMLEN characters.");
	else if (status == ENOENT && operation == UNLINK)
		exit_err("The named semaphore does not exist.");
}

sem_t	*handle_semaphores(sem_t *sem, const char *name,
			unsigned int value, t_sem_ops operation)
{
	sem_t	*ret_value;

	ret_value = NULL;
	if (operation == OPEN)
	{
		ret_value = sem_open(name, O_CREAT, 0644, value);
		if (ret_value == SEM_FAILED)
			check_sem_open_error();
	}
	else if (operation == CLOSE)
		check_sem_error(sem_close(sem), operation);
	else if (operation == POST)
		check_sem_error(sem_post(sem), operation);
	else if (operation == WAIT)
		check_sem_error(sem_wait(sem), operation);
	else if (operation == UNLINK)
		check_sem_error(sem_unlink(name), operation);
	return (ret_value);
}
