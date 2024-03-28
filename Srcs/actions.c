/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 01:29:47 by mawad             #+#    #+#             */
/*   Updated: 2024/03/28 02:42:27 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//&& game_over conditions are there to not print the remaining
//messages if a philo dies.
void	write_status(t_philo *philo, t_philo_status status)
{
	size_t	elapsed;

	elapsed = get_time() - philo->program->start_time;
	handle_mutexes(&(philo->program->write_mutex), LOCK);
	if ((status == FIRST_FORK || status == SECOND_FORK)
		&& !game_over(philo->program))
		printf("%lu""	%d has taken a fork\n",
			elapsed, philo->philo_id);
	else if (status == EATING && !game_over(philo->program))
		printf("%lu""	%d is"" eating\n",
			elapsed, philo->philo_id);
	else if (status == SLEEPING && !game_over(philo->program))
		printf("%lu""	%d is sleeping\n",
			elapsed, philo->philo_id);
	else if (status == THINKING && !game_over(philo->program))
		printf("%lu""	%d is thinking\n",
			elapsed, philo->philo_id);
	else if (status == DEAD && game_over(philo->program))
		printf("%lu""	%d died\n", elapsed, philo->philo_id);
	handle_mutexes(&(philo->program->write_mutex), UNLOCK);
}

//Remember, the philo's can share the same fork (I literally sent
//the same address to more than one philo in my inits.c). So basically,
//Philo 1 and Philo 2 will both have access to fork 1 (way, I wrote the
//code is forks start iterating from 0, so fork 1 is the second fork on
//the table). So anyways, since they both have access to the same fork,
//I need the mutex placed in the address of that fork which would be
//phile->(left_fork or right_fork)->fork. In that scenario (assume philo
//1 won the battle), since I am using fork 1's mutex, then Philo 2 will
//pause up until I unlock fork 1's mutex at the end of the eat function
//that got called by philo 1. So Philo 2 won't be able to continue executing
//its code (it might have already been able to pick up its first fork since
//Philo 2 will try to pick up his right fork first, aka fork 2 if it won that
//battle with Philo 3), but since he lost his battle with Philo 1, he won't be
//able to continue the rest of the eat function up until Philo 1 unlocks that
//fork 1's mutex which happens at the end of the eat function.
//
//The philo->last_meal_time has to be protected with a mutex
//because it will be read by another thread, the monitoring thread
//to check if the elapsed time since the philosopher's last meal
//has exceeded the time to die. Since it will be read by the monitor,
//we want to ensure proper synchronization between the philo thread
//and the monitor thread, so to achieve this synchronization, we use
//the philo_mutex.
void	ft_eat(t_philo *philo)
{
	if (check_full(philo, philo->program))
		return ;
	if ((philo->philo_id % 2) != 0)
		handle_mutexes(&(philo->left_fork->fork), LOCK);
	else
		handle_mutexes(&(philo->right_fork->fork), LOCK);
	write_status(philo, FIRST_FORK);
	if ((philo->philo_id % 2) != 0)
		handle_mutexes(&(philo->right_fork->fork), LOCK);
	else
		handle_mutexes(&(philo->left_fork->fork), LOCK);
	write_status(philo, SECOND_FORK);
	philo->meal_count++;
	write_status(philo, EATING);
	st_writer(&(philo->philo_mutex), &(philo->last_meal_time), get_time());
	ft_usleep(philo->program->time_to_eat, philo->program);
	handle_mutexes(&(philo->right_fork->fork), UNLOCK);
	handle_mutexes(&(philo->left_fork->fork), UNLOCK);
}

void	ft_sleep(t_philo *philo)
{
	if (check_full(philo, philo->program))
		return ;
	write_status(philo, SLEEPING);
	ft_usleep(philo->program->time_to_sleep, philo->program);
}

//If the user decides to run a simulation with like the followig
//./philo 5 400 200 250, then the philosophers will definitely
//die. But the problem is, think_time is cast as size_t and
//in that case think_time = 400 - (200 + 250) which would overflow
//to 18446744073709501616 which is a wait time of 584,942 years LOL. Either
//way, the usleep function, shouldtnt be used for waiting for anything
//larger than 1 second (1,000,000 micro-seconds).
//so we make the comparison before calculating the think_time in case
//it overflows to some insane number. If die_time <= (eat_time + sleep_time)
//then think_time is either 0 or a negative number which overflows to some
//insane number. Hence, there is no need to think anyways because in if
//die_time = eat_time + sleep_time, there is no need to wait anyways and
//if die_time < eat_time + sleep_time, then think_time is negative and the
//philosophers will defintely die anyways, so who cares about setting a think
//time, just return.
void	ft_think(t_philo *philo)
{
	size_t	think_time;
	size_t	die_time;
	size_t	eat_time;
	size_t	sleep_time;

	think_time = 0;
	write_status(philo, THINKING);
	if (philo->program->philo_amnt % 2 == 0)
		return ;
	die_time = st_reader(&(philo->program->read_mutex),
			&(philo->program->time_to_die));
	eat_time = st_reader(&(philo->program->read_mutex),
			&(philo->program->time_to_eat));
	sleep_time = st_reader(&(philo->program->read_mutex),
			&(philo->program->time_to_sleep));
	if (check_full(philo, philo->program))
		return ;
	if (die_time <= (eat_time + sleep_time))
		return ;
	think_time = die_time - (eat_time + sleep_time);
	ft_usleep(0.3 * think_time, philo->program);
}

// void	write_details(t_philo *philo, t_philo_status status)
// {
// 	size_t	elapsed;

// 	elapsed = get_time() - philo->program->start_time;
// 	if (status == FIRST_FORK && !game_over(philo->program))
// 	{
// 		if ((philo->philo_id % 2) != 0)
// 			printf(PRL"%lu ms"OG" philo %d has"YLW" taken"OG" the"
// 				BLU" first fork"OG" 	[fork id: %d]\n",
// 				elapsed, philo->philo_id, philo->left_fork->fork_id);
// 		else
// 			printf(PRL"%lu ms"OG" philo %d has"YLW" taken"OG" the"
// 				BLU" first fork"OG" 	[fork id: %d]\n",
// 				elapsed, philo->philo_id, philo->right_fork->fork_id);
// 	}
// 	else if (status == SECOND_FORK && !game_over(philo->program))
// 	{
// 		if ((philo->philo_id % 2) != 0)
// 			printf(PRL"%lu ms"OG" philo %d has"YLW" taken"OG" the"
// 				RED" second fork"OG" 	[fork id: %d]\n",
// 				elapsed, philo->philo_id, philo->right_fork->fork_id);
// 		else
// 			printf(PRL"%lu ms"OG" philo %d has"YLW" taken"OG" the"
// 				RED" second fork"OG" 	[fork id: %d]\n",
// 				elapsed, philo->philo_id, philo->left_fork->fork_id);
// 	}
// 	else if (status == EATING && !game_over(philo->program))
// 		printf(PRL"%lu ms"OG" philo %d is"GRN" eating 🍜	[meal count: %d]"OG"\n",
// 			elapsed, philo->philo_id, philo->meal_count);
// 	else if (status == SLEEPING && !game_over(philo->program))
// 		printf(PRL"%lu ms"OG" philo %d is sleeping 😴\n",
// 			elapsed, philo->philo_id);
// 	else if (status == THINKING && !game_over(philo->program))
// 		printf(PRL"%lu ms"OG" philo %d is thinking 🏛️\n"OG,
// 			elapsed, philo->philo_id);
// 	else if (status == DEAD && game_over(philo->program))
// 		printf(PRL"%lu ms"OG" philo %d has "RED"died 💀\n",
// 			elapsed, philo->philo_id);
// }
