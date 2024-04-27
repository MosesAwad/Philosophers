/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/13 01:49:34 by mawad             #+#    #+#             */
/*   Updated: 2024/04/24 22:52:32 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//PAY ATTENTION!!!!
//This 	if (philo->philo_id % 2 == 0)
//		ft_usleep(1000, philo->program);
//is arguably the most important line of the code! It fixes the
//issue you face when you insert a number of philosophers larger than
//4 like ./philo 6 410 200 200. So as you can see, the number of
//philos isn't even that large but it is still large enough to
//create an issue. I would recommend you plug the output of
//your code into the philsophers_visualizer available through this
//link:
//https://nafuka11.github.io/philosophers-visualizer/
//to see the issue.
//NOTE: This issue is not always present though, it could differ from
//one time you run the program to the other as it depends on how the
//philos win the fights over the forks (This would hopefully make more
//sense as you read on. but I figured it would be important to give notice
//to this issue here). However, we should find a way to make it work every
//time we run the program. To do that, we use this if-statement, and to see
//why it works, read on.
//Without this if-statement, you would notice that
//the behavior of odd and even philsophers is at an offset, by that
//I mean that some philsophers begin their actions really late. My
//guess is that this is caused by us setting the rule where odd philos
//pick up their left forks first and even philos pick up their right
//forks first. This is good, it avoids deadlocks (if you don't understand
//why, watch the Oceano Philosophers video on Youtube). But this implementation
//does come at a cost, in the scenarios where we have more than 4 philos or
//when we have an odd number of philos.
//In those scenarios, lets say for example when you have more than 4 philos,
//some philosophers can end up picking a fork but losing their next one, and
//because of that fork that this philo picked up, their other neighbor also
//couldn't eat.
//So basically, one philosopher picked up a fork (effectively hindering
//his neighbor), and because he lost the fight over the other fork, he
//now hinders himself. So we end up with two hindered philsophers!
//So to fix this issue, what we do is as soon as the simulation starts, 
//I would make the even philosphers sleep for 1ms (1000 us), something
//negligible, but it makes a world of a difference.
//In that case, we would kinda be cheating for the first round, as
//the odd number of philo's would not have any one to fight with for their
//forks (REMEMBER: ONLY FOR THE FIRST ROUND) because we made the even philos
//sleep. But since they all sleep for only 1ms, when they wake up, that would
//have been enough time for the odd philos to start eating. Then, the even
//philos wait till it becomes their turn to eat, and now, the simulation gets
//back to normal (whereby the philos actually fight over the forks when it
//becomes they are awake starting from the SECOND round and for every other
//round afterwards). This steps greatly improves the synchronicity between
//the philos and fixes the issue mentioned earlier.
static void	*dinner_simulation(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
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

//Technically there is no last meal time since a solo philosopher
//cannot eat (due to their only being one fork present). But, the
//last_meal_time attribute is set just so that it works properly with
//the way the monitor is designed (specifcally the philo_lives helper
//function)
static void	*solo_philo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	handle_mutexes(&(philo->right_fork->fork), LOCK);
	write_status(philo, FIRST_FORK);
	philo->last_meal_time = get_time();
	while (!game_over(philo->program))
		;
	handle_mutexes(&(philo->right_fork->fork), UNLOCK);
	return (NULL);
}

//The bool_writer(&(program->read_mutex), &(program->game_over), TRUE);
//is to handle the case where all the philos get full. In that case, we
//only break out of the corresponding dinner_simulation loops of each
//philo. So if we break out of all the corresponding loops, then in that
//case, we reach the bool_writer(&(program->read_mutex),
//&(program->game_over), TRUE); line which sets the game_over bool to TRUE.
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
//bool_writer(&(program->read_mutex), &(program->game_over), TRUE); which is
//how we handle setting that bool to TRUE if all the philos are now full. Then
//we can call:
//	handle_threads(&(program->monitor), NULL,
//		NULL, JOIN);
//which at that point kinda does nothing because by setting the game_over bool
//to TRUE in dinner_commence, we effectively broke out of the while (!game_over)
//loop in the monitor_simulation function and thus returned NULL. Meaning that
//as soon as:
// handle_threads(&(program->monitor), NULL,
// 	NULL, JOIN);
//is called in dinner_commence(), it has already terminated; but still, it is
//better practice to pthread_join on the monitor_simulation thread. The main
//point being is that the bool_writer(&(program->read_mutex),
//&(program->game_over), TRUE); has to be exactly where it is and not for
//example, below the:
// handle_threads(&(program->monitor), NULL, NULL, JOIN);
//line. Otherwise, the main process would continue to wait on the
//monitor_simuation thread and the bool_writer(&(program->read_mutex),
//&(program->game_over), TRUE); line (if it was below) never being reached
//due to the while (!game_over(program)) loop running continously as nothing
//ever set the game_over bool to TRUE.
void	dinner_commence(t_program *program)
{
	int	i;

	i = 0;
	program->start_time = get_time();
	if (program->philo_amnt == 1)
	{
		handle_threads(&(program->philos[i].thread), solo_philo,
			&(program->philos[i]), CREATE);
		i++;
	}
	while (i < program->philo_amnt)
	{
		handle_threads(&(program->philos[i].thread), dinner_simulation,
			&(program->philos[i]), CREATE);
		i++;
	}
	handle_threads(&(program->monitor), monitor_simulation, program, CREATE);
	i = 0;
	while (i < program->philo_amnt)
		handle_threads(&(program->philos[i++].thread), NULL,
			NULL, JOIN);
	bool_writer(&(program->read_mutex), &(program->game_over), TRUE);
	handle_threads(&(program->monitor), NULL,
		NULL, JOIN);
}
