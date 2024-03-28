/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 02:06:23 by mawad             #+#    #+#             */
/*   Updated: 2024/03/27 01:03:35 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	exit_err(char *message)
{
	printf(RED"⚠️ %s ⚠️\n"OG, message);
	exit(1);
}

void	*ft_alloc(size_t size)
{
	void	*buffer;

	buffer = malloc(size);
	if (buffer == NULL)
		exit_err("Insufficient space.");
	return (buffer);
}

int	ft_atoi(char *str)
{
	int	i;
	int	flag;
	int	result;

	i = 0;
	flag = 1;
	result = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i++] == '-')
			flag *= -1;
	}
	while (str[i] >= '0' && str[i] <= '9')
		result = result * 10 + (str[i++] - '0');
	return (result * flag);
}

void	ft_destroy(t_program *program)
{
	// handle_mutexes(&(program->read_mutex), DESTROY);
	// handle_mutexes(&(program->write_mutex), DESTROY);
	handle_semaphores(program->write_sem, NULL, 0, CLOSE);
	// while (i < program->philo_amnt)
	// 	handle_mutexes(&(program->forks[i++].fork), DESTROY);
	handle_semaphores(program->forks, NULL, 0, CLOSE);
	free(program->philos);
}
