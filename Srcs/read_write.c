/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_write.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 01:46:30 by mawad             #+#    #+#             */
/*   Updated: 2024/03/23 00:13:50 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_bool	bool_reader(pthread_mutex_t *mutex, t_bool *read)
{
	t_bool	buffer;

	handle_mutexes(mutex, LOCK);
	buffer = *read;
	handle_mutexes(mutex, UNLOCK);
	return (buffer);
}

void	bool_writer(pthread_mutex_t *mutex, t_bool *dest, t_bool new_value)
{
	handle_mutexes(mutex, LOCK);
	*dest = new_value;
	handle_mutexes(mutex, UNLOCK);
}

size_t	st_reader(pthread_mutex_t *mutex, size_t *read)
{
	size_t	buffer;

	handle_mutexes(mutex, LOCK);
	buffer = *read;
	handle_mutexes(mutex, UNLOCK);
	return (buffer);
}

void	st_writer(pthread_mutex_t *mutex, size_t *dest, size_t new_value)
{
	handle_mutexes(mutex, LOCK);
	*dest = new_value;
	handle_mutexes(mutex, UNLOCK);
}
