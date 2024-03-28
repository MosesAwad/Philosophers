/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 23:34:01 by mawad             #+#    #+#             */
/*   Updated: 2024/03/28 23:21:30 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

//The kill semaphore is present so that the program does
//not give out more than one 'philo x has died' message.
//With this, whichever philo dies first will terminate the
//rest of the processes via the SIGINT signal (except itself)
//while having a monopoly over this section of the code. Because
//it can happen that more than one philo dies at the same instance
//which thereby gives more than one 'philo x has died' message
//and it also messes up the termination of the res tof the processes
//because then, we would then have multiple processes trying to close
//other processes (including trying to close one another) which is just
//a mess. So this semaphore called kill_sem corrects the termination process
//and makes it neater.
//This loop
//		while (i < program->philo_amnt)
// 		{
// 			if ((i++) != program->philo_data.philo_id - 1)
// 				kill(program->philos[i - 1], SIGINT);
// 		}
//looks complicated but is not. It just saying terminate every other process
//except the current one via SIGINT. I did (i++) to bypass norm restrictions
//and then said instead kill(program->philos[i - 1], SIGINT). It is the same
//loop as
//		while (i < program->philo_amnt)
// 		{
// 			if (i != program->philo_data.philo_id - 1)
// 				kill(program->philos[i], SIGINT);
//			i++;
// 		}
//I am comparing i to program->philo_data.philo_id - 1 because the processes
//are counted from 0 upwards while the philo_id's are counted from 1 upwards.
//That's it, it's not that complicated.
t_bool	philo_survives(t_program *program)
{
	size_t		lmt;
	size_t		elapsed;
	int			i;

	i = 0;
	handle_semaphores(program->philo_data.read_sem, NULL, 0, WAIT);
	lmt = program->philo_data.last_meal_time;
	handle_semaphores(program->philo_data.read_sem, NULL, 0, POST);
	elapsed = get_time() - lmt;
	if (elapsed > program->time_to_die / 1e3)
	{
		program->write_sem = handle_semaphores(program->kill_sem, NULL,
				0, WAIT);
		write_status(program, DEAD);
		bool_writer(program->philo_data.end_sem, &(program->game_over), TRUE);
		while (i < program->philo_amnt)
		{
			if ((i++) != program->philo_data.philo_id - 1)
				kill(program->philos[i - 1], SIGINT);
		}
		program->write_sem = handle_semaphores(program->kill_sem, NULL,
				0, POST);
		return (FALSE);
	}
	return (TRUE);
}

void	*monitor_simulation(void *arg)
{
	t_program	*program;
	size_t		lmt;

	program = (t_program *)arg;
	while (1)
	{
		handle_semaphores(program->philo_data.read_sem, NULL, 0, WAIT);
		lmt = program->philo_data.last_meal_time;
		handle_semaphores(program->philo_data.read_sem, NULL, 0, POST);
		if (lmt != 0)
			break ;
	}
	while (!game_over(program))
	{
		if (philo_survives(program) == FALSE)
			return (NULL);
	}
	return (NULL);
}
