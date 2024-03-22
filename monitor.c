/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 23:24:40 by mawad             #+#    #+#             */
/*   Updated: 2024/03/23 02:04:30 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//lmt stands for last meal time. I have to wait till its
//initialized before I can start the monitor. Because if
//it is not, then undefined behavior would occur
//in the if (elapsed > (program->time_to_die / 1e3))
//condition in my monitor_simulation loop.
static t_bool	all_lmts_initialized(t_program *program)
{
	if (st_reader(&(program->read_mutex), &(program->lmt_init_count))
		== (size_t) program->philo_amnt)
		return (true);
	return (false);
}

static t_bool	philo_lives(size_t elapsed, int i, t_program *program)
{
	if (elapsed > (program->time_to_die / 1e3)
		&& !check_full(&(program->philos[i]), program))
	{
		bool_writer(&(program->read_mutex),
			&(program->game_over), true);
		write_status(&(program->philos[i]), DEAD);
		return (false);
	}
	return (true);
}

//No need for a spinlock here that waits for all the threads to be running.
//The monitor_simulation thread begins only after ensuring that the
//program->all_threads_ready bool is set true in the dinner_commence() function.
//So the threads have definetely been created by now, in other words, st_writer
//here won't be writing to a philo->sub_thread that hasn't been created yet so
//its fine.
void	*monitor_simulation(void *arg)
{
	t_program	*program;
	size_t		elapsed;
	int			i;

	program = (t_program *)arg;
	i = 0;
	while (all_lmts_initialized(program) == false)
	{
		printf("p");
	}
	while (!game_over(program))
	{
		i = 0;
		while (i < program->philo_amnt)
		{
			elapsed = get_time() - st_reader(&(program->philos[i].philo_mutex),
					&(program->philos[i].last_meal_time));
			if (philo_lives(elapsed, i, program) == false)
				return (NULL);
			i++;
		}
	}
	return (NULL);
}
