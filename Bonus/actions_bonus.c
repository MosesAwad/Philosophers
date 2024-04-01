/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 00:00:44 by mawad             #+#    #+#             */
/*   Updated: 2024/04/02 01:03:19 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

// void	write_status(t_program *program, t_philo_status status)
// {
// 	size_t	elapsed;

// 	elapsed = get_time() - program->start_time;
// 	printf("get time from philo %d is %lu\n", program->philo_data.philo_id, get_time());
// 	handle_semaphores(program->write_sem, NULL, 0, WAIT);
// 	if ((status == FIRST_FORK || status == SECOND_FORK)
// 		&& !game_over(program))
// 		printf("%lu""	%d has taken a fork at time %lu\n",
// 			elapsed, program->philo_data.philo_id, get_time());
// 	else if (status == EATING && !game_over(program))
// 		printf("%lu""	%d is"" eating at time %lu\n",
// 			elapsed, program->philo_data.philo_id, get_time());
// 	else if (status == SLEEPING && !game_over(program))
// 		printf("%lu""	%d is sleeping at time %lu\n",
// 			elapsed, program->philo_data.philo_id, get_time());
// 	else if (status == THINKING && !game_over(program))
// 		printf("%lu""	%d is thinking at time %lu\n",
// 			elapsed, program->philo_data.philo_id, get_time());
// 	else if (status == DEAD && game_over(program) == TRUE)
// 		printf(RED"%lu""	%d died\n"OG, elapsed, program->philo_data.philo_id);
// 	handle_semaphores(program->write_sem, NULL, 0, POST);
// }

void	write_status(t_program *program, t_philo_status status)
{
	size_t	elapsed;

	elapsed = get_time() - program->start_time;
	handle_semaphores(program->write_sem, NULL, 0, WAIT);
	if ((status == FIRST_FORK || status == SECOND_FORK)
		&& !game_over(program))
		printf("%lu""	%d has taken a fork\n",
			elapsed, program->philo_data.philo_id);
	else if (status == EATING && !game_over(program))
		printf("%lu""	%d is"" eating\n",
			elapsed, program->philo_data.philo_id);
	else if (status == SLEEPING && !game_over(program))
		printf("%lu""	%d is sleeping\n",
			elapsed, program->philo_data.philo_id);
	else if (status == THINKING && !game_over(program))
		printf("%lu""	%d is thinking\n",
			elapsed, program->philo_data.philo_id);
	else if (status == DEAD && game_over(program) == TRUE)
		printf(RED"%lu""	%d died\n"OG, elapsed, program->philo_data.philo_id);
	handle_semaphores(program->write_sem, NULL, 0, POST);
}

//The balance semaphore is important here because in the bonus part,
//all the forks are placed in the middle of the table (as reflected by
//the nature of semaphores). So, what if I have 4 philos and this
//happens: philo 1 takes both forks and begins eating at 0 ms.
//But at 0 ms, philo 2 and philo 3 each take one fork 
//(this can happen because as I said, all forks are
//placed in the middle of the table, so all the forks are up for grabs
//unlike the mandatory part where each philo can only pick the fork to
//his left or the fork to his right, so there will always be a winner
//in every 2 philos. But here, there was only one winner amongst 4
//philos). So now we are out of forks and philo 2, 3, and 4 aren't
//doing anything. That's inefficient! If there are 4 forks, then 2
//philos should be eating at an instance. So to prevent this from
//happening, I basically force the philo pick up 2 forks (any 2 but it
//has to be 2) everytime he enters the ft_eat function. This is to
//guarantee that no 2 neighboring philos will pick up only one fork at 
//a given instance. The value of this balance semaphore is half the amount
//of the philosophers because if I have 4 philosophers thus 4 forks, then 
//logically any 2 can eat at the same time. Similarly, if I have 5 philos
//thus 5 forks, then logically any 2 can eat at the same time (5/2 = 2.5
//truncated to 2 by the compiler automatically) since the extra fork wont
//be able to be used for eating because each philo can only eat after
//obtaining 2 forks). That extra fork though won't be picked up by anyone
//in that instance (unlike in the mandatory part where a philo can simply
//grab it and wait to grab the other fork) because in the bonus part (due
//to the nature of how semaphores work), I can only allow each philo to
//pick up two forks at one instance, otherwise the aforementioned
//inefficiency would occur.
void	ft_eat(t_program *program)
{
	handle_semaphores(program->balance_sem, NULL, 0, WAIT);
	write_status(program, FIRST_FORK);
	write_status(program, SECOND_FORK);
	write_status(program, EATING);
	handle_semaphores(program->philo_data.read_sem, NULL, 0, WAIT);
	program->philo_data.last_meal_time = get_time();
	handle_semaphores(program->philo_data.read_sem, NULL, 0, POST);
	ft_usleep(program->time_to_eat, program);
	handle_semaphores(program->balance_sem, NULL, 0, POST);
}

void	ft_sleep(t_program *program)
{
	write_status(program, SLEEPING);
	ft_usleep(program->time_to_sleep, program);
}

void	ft_think(t_program *program)
{
	write_status(program, THINKING);
}

// void	ft_think(t_program *program)
// {
// 	size_t	think_time;
// 	size_t	die_time;
// 	size_t	eat_time;
// 	size_t	sleep_time;

// 	think_time = 0;
// 	write_status(program, THINKING);
// 	die_time = program->time_to_die;
// 	eat_time = program->time_to_eat;
// 	sleep_time = program->time_to_sleep;
// 	// if (check_full(philo, philo->program))
// 	// 	return ;
// 	if (die_time <= (eat_time + sleep_time))
// 		return ;
// 	think_time = die_time - (eat_time + sleep_time);
// 	ft_usleep(0.3 * think_time, program);
// }
