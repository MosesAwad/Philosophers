/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 23:34:01 by mawad             #+#    #+#             */
/*   Updated: 2024/04/14 19:13:53 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

//Here we use the wait_status semaphore to prevent other
//philos from writing messages after a philo dies. So basically
//what can happen is philo 7 can die but the other philos can
//still be running and display their messages. That should not
//be the case. So, to prevent that, we simply just wait (aka lock)
//the write sem. In the mandatory part, the equivalent of doing this
//was to use the (&& !game_over(philo->program)) condition in the
//write_status() function. But, I have decided not to go for using
//the game_over() boolean check function anymore for the bonus part.
//I can just use a while (1) loop instead.
//
//Side note: we post the wait_sem semaphore in the wait_philos()
//function found in main_bonus.c. Technically, we do not need to
//post it anymore since by then, we would be preparing for terminating
//the program and all the processes/philos would have been terminated by
//SIGINT but hey, why not get rid of that itch lol.
void	end_sim(t_program *program)
{
	size_t	elapsed;

	elapsed = get_time() - program->start_time;
	handle_semaphores(program->write_sem, NULL, 0, WAIT);
	printf("%lu""	%d died\n", elapsed,
		program->philo_data.philo_id);
	if (program->meal_limit != -1)
	{
		handle_semaphores(program->philo_data.philo_sem, NULL, 0, WAIT);
		program->philo_data.meal_count = program->meal_limit;
		handle_semaphores(program->philo_data.philo_sem, NULL, 0, POST);
		handle_semaphores(program->write_sem, NULL, 0, POST);
	}
	handle_semaphores(program->wait_sem, NULL, 0, POST);
}

t_bool	philo_survives(t_program *program)
{
	size_t		lmt;
	size_t		elapsed;

	handle_semaphores(program->philo_data.philo_sem, NULL, 0, WAIT);
	lmt = program->philo_data.last_meal_time;
	handle_semaphores(program->philo_data.philo_sem, NULL, 0, POST);
	elapsed = get_time() - lmt;
	if (elapsed > program->time_to_die / 1e3)
	{
		end_sim(program);
		return (FALSE);
	}
	return (TRUE);
}

//Place a usleep in the monitor to lower the load on the semaphore.
//The semaphore has to be for interpertation of the last_meal_time
//variable because the main thread (where the philo eats, sleeps, and
//thinks) would be altering the last_meal_time in ft_eat. So, what could
//happen if there is no semaphore is the main_thread can alter the variable
//last_meal_tme but the monitor thread could be paused (by the system for
//whatever reason) and thus, it could pick the old value that hasn't been
//updated yet! So, what needs to happen is, we need to use  asynchornization
//tool (for the mandatory parts, we use mutexes and for the bonus, we use
//semaphores) such that if the main thread was currently altering the variable
//last_meal_time, then the monitor thread has to wait till the main_thread
//posts it before it can proceed to reading it. By doing that, we ensure that
//last_meal_time is now the most_recent_value!
//However, if we do not use a usleep in here, the monitor thread would be
//running millions of times a second, a while (1) loop is fast! So the main
//thread (when it is not sleeping for eating, sleeping, and thinking)
//and the monitor thread would constantly be waiting and posting this
//semaphore non-stop which causes a huge delay in the system! Since we only have
//a leniency of 10ms by the subject pdf, I decided to bypass this problem by
//making the monitor thread sleep for 1000 microseconds only, so 0.001 seconds!
//So if we are late by that, to detect whether a philo hasn't survived, it would
//be a negligible difference! BUT, to the system it is a huge difference as now,
//the program is indeed much faster and I am not experiencing this crazy delay
//anymore. And better yet, we do not have any data races since we are, in fact,
//implementing a semaphore on the interpetation of the last_meal_time variable!
void	*monitor_simulation(void *arg)
{
	t_program	*program;
	size_t		lmt;

	program = (t_program *)arg;
	while (1)
	{
		usleep(1000);
		handle_semaphores(program->philo_data.philo_sem, NULL, 0, WAIT);
		lmt = program->philo_data.last_meal_time;
		handle_semaphores(program->philo_data.philo_sem, NULL, 0, POST);
		if (lmt != 0)
			break ;
	}
	while (1 && check_full(program) == FALSE)
	{
		usleep(1000);
		if (philo_survives(program) == FALSE)
			return (NULL);
	}
	return (NULL);
}
