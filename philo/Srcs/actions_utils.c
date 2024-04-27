/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 21:59:56 by mawad             #+#    #+#             */
/*   Updated: 2024/04/13 22:02:12 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	ft_pickup_first_fork(t_philo *philo)
{
	if ((philo->philo_id % 2) != 0)
	{
		while (bool_reader(&(philo->left_fork->fork),
				&(philo->left_fork->value)) == 1)
			ft_usleep(1000, philo->program);
		bool_writer(&(philo->left_fork->fork), &(philo->left_fork->value), 1);
	}
	else
	{
		while (bool_reader(&(philo->right_fork->fork),
				&(philo->right_fork->value)) == 1)
			ft_usleep(1000, philo->program);
		bool_writer(&(philo->right_fork->fork), &(philo->right_fork->value), 1);
	}
	write_status(philo, FIRST_FORK);
}

static void	ft_pickup_second_fork(t_philo *philo)
{
	if ((philo->philo_id % 2) != 0)
	{
		while (bool_reader(&(philo->right_fork->fork),
				&(philo->right_fork->value)) == 1)
			ft_usleep(1000, philo->program);
		bool_writer(&(philo->right_fork->fork), &(philo->right_fork->value), 1);
	}
	else
	{
		while (bool_reader(&(philo->left_fork->fork),
				&(philo->left_fork->value)) == 1)
			ft_usleep(1000, philo->program);
		bool_writer(&(philo->left_fork->fork), &(philo->left_fork->value), 1);
	}
	write_status(philo, SECOND_FORK);
}

void	ft_pickup_forks(t_philo *philo)
{
	ft_pickup_first_fork(philo);
	ft_pickup_second_fork(philo);
}
