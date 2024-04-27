/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 00:27:08 by mawad             #+#    #+#             */
/*   Updated: 2024/04/08 23:04:02 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

//The usage of 	handle_semaphores(program->write_sem, NULL, 0, POST);
//is explained above the end_sim() function present in monitor_bonus.c.
//The point of the wait_philos function is as follows: to "artifically"
//make the main processes wait for its child processes (the philosophers)
//through a semaphore, as opposed to the waitpid() function. So, the
//usage of the waitpid function itself is no big deal, we can use it.
//But the issue arises when we get to using the kill function. We need
//to use the kill function to terminate all the philosopher processes
//remaining as soon as one of the philosophers does not survive a round.
//The problem though is that kill requires the pid's of the processes
//as its first argument. So, when we fork in set_up_procs() function
//found in simulation_bonus.c, we do it as follows program->philos[i] = fork().
//So what's the big deal? Well, the only process that ends up holding all the
//process id's is actually the main process only! Let's say we have 4 philos.
//Then, as soon as philo 1's process starts, it inherits this as its
//program.philos array (where X stands for uninitialized and the numbers are
//arbitary values for the assigned pid's):
//program.philos (in philo 1 process): [0, X, X, X]
//
//Next, philo 2 inherits this:
//program.philos (in philo 2 process): [69, 0, X, X] 
//(where 69 is the pid of the philo 1 child process and
//0 is its ow pid (from its perspective but from the perspective of the main
//process it will be 70 (which is the next number in line)))
//
//Next, philo 3 inherits this:
//program.philos (in philo 3 process): [69, 70, 0, X]
//(69 is the pid of philo 1 and 70 is the pid of philo 2 child processes)
//
//Next, philo 4 inherits this:
//program.philos (in philo 4 process): [69, 70, 71, 0] (you get the idea).
//
//So, only the main process (which has not gotten forked) is the one with access
//to the actual pid's of all the child processes!
//Philos array in the main process:
//program.philos: [69, 70, 71, 72]
//So, in order to actually kill all the processes, we have to do it from main
//process. We can't just say from inside one of the child/philo processes to
//kill all the other philo processes. Because if for instance, philo 2 does
//not survive, how would it be able to kill philo 3, and philo 4 if it
//does not have access to their pid's? ALl it has access to is the pid
//of philo 1 (Even its own pid it does not have because fork returned 0 to it,
//not its own pid, its pid is only returned to the main process). It can still
//terminate itself by calling exit() nonetheless so it does not really need its
//own pid, but it would never be able to kill philo 3 and philo 4.
//Remember, from the perspective of philo 2, this is how philo.program looked
//like when it inherited it:
//program.philos: [69, 0, X, X]
//So, the flag that we are using from the main process to kill all the processes
//since one of the philo's did not survive is the wait_sem semaphore! If you go
//to monitor_bonus.c and then to the end_sim() function, you would see that
//that's when we post the wait_sem, aka release it. Meanwhile here in
//wait_philos, the main process was on pause because we called
//handle_semaphores(program->wait_sem, NULL, 0, WAIT); twice! Since we
//initialize it to value 1, the first call lowers it to 0, and then the second
//call would pause the whole process (because execution of the code cannot
//continue when a sem_wait call is made on a semaphore whose value is at 0 
//until the semaphore is released via sem_post, and this sem_post is executed
//by the philosopher that does not survive a round).
//However, you do see that I am also using waitpid. So,
//if (program->meal_limit == -1), then I know that the user did not insert a
//meal count. In my init_program() function in inits_bonus.c, you can see that.
//I choose -1 as the flag to indicate that there is no meal_limit. So, in that
//case, the main process would wait via the semaphore technqiue I explained
//above. Otherwise, if the user did in fact input a meal count, then I will
//go for the waitpid option instead. Why? If you go to the dinner_simulation()
//function in simulation_bonus.c, you would see that we have, inside the
//while (1) loop, the following condition:
// if (check_full(program) == TRUE)
// 	break ;
//So if a particular philo is full, then we proceed to exit the program
//via exit(0) after a few clean_up lines. Now assume that I am using the
//wait_sem technique for this scenario too so that I can explain to you
//why it does not work when a meal_limit is inserted by the user. Logically,
//we would have to post the wait_sem in the philo_process that finished
//eating all his meals before we get to exit(0). The issue, though, is that
//not all philos would finish eating all their meals at the same time. But,
//all it takes is one post operation to stop the simulated waiting process
//of the semaphore to get the main process to continue executing its code.
//So, if I only had this loop:
// handle_semaphores(program->wait_sem, NULL, 0, WAIT);
// handle_semaphores(program->wait_sem, NULL, 0, WAIT);
// while (i < program->philo_amnt)
// {
// 	kill(program->philos[i], SIGINT);
// 	i++;
// }
// handle_semaphores(program->write_sem, NULL, 0, POST);
//then as soon as one philo finishes eating, I would proceed to prematurely
//kill all the other philo processes even if they haven't finished eating
//all their meals!! So in the scenario where we have a meal_limit imposed
//by the user, then just proceed to use waitpid, each processes would
//automatically exit via exit(0) when all the meals have been eaten and
//the main process would proceed to continue after all the prcoesses have
//exited.
void	wait_philos(t_program *program)
{
	int	i;

	i = 0;
	if (program->meal_limit == -1)
	{
		handle_semaphores(program->wait_sem, NULL, 0, WAIT);
		handle_semaphores(program->wait_sem, NULL, 0, WAIT);
		while (i < program->philo_amnt)
		{
			kill(program->philos[i], SIGINT);
			i++;
		}
		handle_semaphores(program->write_sem, NULL, 0, POST);
	}
	else
	{
		while (i < program->philo_amnt)
			waitpid(program->philos[i++], NULL, 0);
	}
}

int	main(int argc, char *argv[])
{
	t_program	program;

	if (!(argc == 5 || argc == 6))
	{
		printf("Please insert 5 or 6 strict arguments only\n");
		return (1);
	}
	parser(argv);
	init_program(&program, argv);
	set_up_procs(&program);
	wait_philos(&program);
	ft_destroy(&program);
	return (0);
}
