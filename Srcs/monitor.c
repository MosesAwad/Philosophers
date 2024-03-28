/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 23:24:40 by mawad             #+#    #+#             */
/*   Updated: 2024/03/26 00:26:43 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//I believe what was happening when I did 
//elapsed = get_time() - st_reader(&(program->philos[i].philo_mutex),
//	&(program->philos[i].last_meal_time)); directly, get_time() got 
//the time but since the monitor thread works concurrently at the same
//time with the philosopher threads, last_meal_time might have not been
//written yet by ft_eat of that philo or it could have been locked for
//a very slight duration by that philo's mutex while we were using 
//st_writer(&(philo->philo_mutex), &(philo->last_meal_time), get_time());
//in ft_eat. So that slight delay would have set get_time() behind the
//assignment of the philo->last_meal_time and since we're dealing with
//milliseconds, that slight delay was causing philosphers to die when
//they shouldnt because elapsed get_time() was smaller than last_meal_time
//and since elapsed is size_t, it would overflow to a huge number mistakenly
//and so, the philospher dies because that larger overflowed number is larger
//than (program->time_to_die / (size_t) 1e3). So, the best way to fix this
//is to first store the result of st_reader(&(program->philos[i].philo_mutex),
//	&(program->philos[i].last_meal_time));  in a variable called lmt, aka
//"last meal time" and let the mutex delays take their course. Then we do
//elapsed = get_time() - lmt; and at that point, we are to be sure that
//get_time() is larger than lmt because we are calulcating it after the
//mutex handles the write (from ft_eat) / read (from here) synchronizations
//so get_time() would actually be the proper current time instance from
//the perspective of the monitor.
static t_bool	philo_lives(int i, t_program *program)
{
	size_t	elapsed;
	size_t	lmt;

	lmt = st_reader(&(program->philos[i].philo_mutex),
			&(program->philos[i].last_meal_time));
	elapsed = get_time() - lmt;
	if (elapsed > (program->time_to_die / (size_t) 1e3)
		&& !check_full(&(program->philos[i]), program))
	{
		// printf("elapsed is %lu, the result of time %lu - lmt %lu\n", elapsed, get_time(), lmt);
		bool_writer(&(program->read_mutex),
			&(program->game_over), TRUE);
		write_status(&(program->philos[i]), DEAD);
		return (FALSE);
	}
	return (TRUE);
}

//No need for a spinlock here that waits for all the threads to be running.
//The monitor_simulation thread begins only after ensuring that the
//program->all_threads_ready bool is set TRUE in the dinner_commence() function.
//So the threads have definetely been created by now, in other words, st_writer
//here won't be writing to a philo->sub_thread that hasn't been created yet so
//its fine.
void	*monitor_simulation(void *arg)
{
	t_program	*program;
	int			i;

	program = (t_program *)arg;
	i = 0;
	while (!game_over(program))
	{
		i = 0;
		while (i < program->philo_amnt && program->philos[i].last_meal_time)
		{
			if (philo_lives(i, program) == FALSE)
				return (NULL);
			i++;
		}
	}
	return (NULL);
}
