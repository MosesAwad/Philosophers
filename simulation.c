/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 01:49:34 by mawad             #+#    #+#             */
/*   Updated: 2024/03/23 02:04:50 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//This function is there to ensure that dinner simulation begins
///when all the threads have been created. It is just to synchornize
//the beginining of the simulation across all philos by ensuring that
//no one attempts to begin (aka attempt to start eating) until all
//the threads have been created. Now whether they actually start
//execution or not is random tbh and that we cannot control, but we
//need to at least make sure that all the threads exist before beginning
//to give all philos an equal chance at the start.
//Now whether they start executing or not, ye96efel menno la rabbo, ana
//shu da5alni? El mohem 3a6ai6o for9a w howwe yedabber 7alo. The point
//being is if I dont have this spinlock, then some greedy ass philos might
//start the simulation (aka attempt to eat by picking up a fork) before the
//other philos even exist lmao.
void	spinlock_all_threads(t_program *program)
{
	t_bool	reader;

	reader = false;
	while (reader == false)
	{
		printf("l");
		handle_mutexes(&(program->read_mutex), LOCK);
		reader = program->all_threads_ready;
		handle_mutexes(&(program->read_mutex), UNLOCK);
	}
}

void	*dinner_simulation(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	spinlock_all_threads(philo->program);
	handle_mutexes(&(philo->program->read_mutex), LOCK);
	st_writer(&(philo->philo_mutex),
		&(philo->last_meal_time), get_time());
	philo->program->lmt_init_count++;
	handle_mutexes(&(philo->program->read_mutex), UNLOCK);
	if (philo->philo_id % 2 == 0)
		ft_usleep(1000, philo->program);
	while (!game_over(philo->program))
	{
		if (check_full(philo, philo->program))
			break ;
		ft_eat(philo);
		ft_sleep(philo);
		ft_think(philo);
	}
	return (NULL);
}

//The bool_writer(&(program->read_mutex), &(program->game_over), true);
//is to handle the case where all the philos get full. In that case, we
//only break out of the corresponding dinner_simulation loops of each
//philo. So if we break out of all the corresponding loops, then in that
//case, we reach the bool_writer(&(program->read_mutex),
//&(program->game_over), true); line which sets the game_over bool to true.
//If we did not do that, then the monitor_simulation would get stuck in its
//own while (!game_over(program)). Also just fyi, pthread_join does not start
//the thread. It only waits for it to terminate. If we did not have it, then
//the main process would just exit dinner_commence and terminate thereby
//terminating all its threads along with it. That would be bad because
//we have while (!game_over) loops in both our dinner_simulation and
//monitor_simulation functions called by the subthreads. So, the threads
//actually start as soon as we hit pthread_create. The pthread_join calls
//are exactly like wait() but for threads. So, the way it works is when
//we do:
// if (check_full(philo, philo->program))
//	 break ;
//in dinner_simulation, we are effectively exiting out of the dinner_simulation
//function, terminating the thread manually, by exiting the while (!game_over)
//loop and going to return (NULL). So, once all of them terminate, only then
//do we exit out of the:
// while (i < program->philo_amnt)
// {
// 	handle_threads(&(program->philos[i].thread), NULL,
// 		NULL, JOIN);
// 	i++;
// }
//loop in the dinner_commence function. So that's when we set
//bool_writer(&(program->read_mutex), &(program->game_over), true); which is
//how we handle setting that bool to true if all the philos are now full. Then
//we can call:
//	handle_threads(&(program->monitor), NULL,
//		NULL, JOIN);
//which at that point kinda does nothing because by setting the game_over bool
//to true in dinner_commence, we effectively broke out of the while (!game_over)
//loop in the monitor_simulation function and thus returned NULL. Meaning that
//as soon as:
// handle_threads(&(program->monitor), NULL,
// 	NULL, JOIN);
//is called in dinner_commence(), it has already terminated; but still, it is
//better practice to pthread_join on the monitor_simulation thread. The main
//point being is that the bool_writer(&(program->read_mutex),
//&(program->game_over), true); has to be exactly where it is and not for
//example, below the:
// handle_threads(&(program->monitor), NULL, NULL, JOIN);
//line. Otherwise, the main process would continue to wait on the
//monitor_simuation thread and the bool_writer(&(program->read_mutex),
//&(program->game_over), true); line (if it was below) never being reached
//due to the while (!game_over(program)) loop running continously as nothing
//ever set the game_over bool to true.
void	dinner_commence(t_program *program)
{
	int	i;

	i = 0;
	while (i < program->philo_amnt)
	{
		handle_threads(&(program->philos[i].thread), dinner_simulation,
			&(program->philos[i]), CREATE);
		i++;
	}
	program->start_time = get_time();
	handle_mutexes(&(program->read_mutex), LOCK);
	program->all_threads_ready = true;
	handle_mutexes(&(program->read_mutex), UNLOCK);
	handle_threads(&(program->monitor), monitor_simulation, program, CREATE);
	i = 0;
	while (i < program->philo_amnt)
		handle_threads(&(program->philos[i++].thread), NULL,
			NULL, JOIN);
	bool_writer(&(program->read_mutex), &(program->game_over), true);
	handle_threads(&(program->monitor), NULL,
		NULL, JOIN);
}
