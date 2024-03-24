/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 00:08:23 by mawad             #+#    #+#             */
/*   Updated: 2024/03/24 03:07:37 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

//"bool_reader(&(program->read_mutex), &(program->game_over))" is just
//too long of a line to be called in our functions later on (namely the
//write_status, write_details (for debugging), monitor_simulation, and
//dinner_simulation). So, to make the code look more presentable and such
//that norminette doesn't start nagging, we create this function to make
//the bool_reader() call even shorter.
t_bool	game_over(t_program *program)
{
	return (bool_reader(&(program->read_mutex), &(program->game_over)));
}

t_bool	check_full(t_philo *philo, t_program *program)
{
	handle_mutexes(&(program->read_mutex), LOCK);
	if (philo->meal_count == program->meal_limit)
	{
		bool_writer(&(philo->philo_mutex), &(philo->philo_full), true);
		return (handle_mutexes(&(program->read_mutex), UNLOCK), true);
	}
	return (handle_mutexes(&(program->read_mutex), UNLOCK), false);
}

//Gives the current time in milliseconds since the UNIX epoch.
size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + (tv.tv_usec / 1000));
}

//Problem with the original usleep is that it sleeps for
//AT LEAST the amount of time we give it but not exactly.
//So it could end up sleeping for a little more than what
//we had intended it to sleep for. This can lead to inaccuracies
//when running our philosophers program. So, ft_usleep is a more
//accurate version of the original usleep. The way ft_usleep
//works is actually quite simple. We choose an arbitrary value
//up until which we run the actual usleep function. So in this
//case, I chose the arbitrary value as 1ms. So let's say I
//decide to use ft_usleep for the literal sleeping process. If
//the user sets time to sleep to 200ms, Then I need to sleep
//for 200ms * 10 ^ 3 aka 200,000 microseconds. Then, since
//my arbitrary value is 1ms and thus 1000microseconds. Then
//from 0 microseconds (0 being the reference point from whence
//the ft_usleep was first called) up until 199,000 microseconds, I will
//use the actual usleep. So let's say 0 microseconds have elapsed,
//our remaining time till the end of the ft_usleep duration would be
//200,000 microseconds. So I will sleep for a quarter of that period.
//So we will usleep for 50,000 microseconds. Why quarter? Because I want
//to allow for a buffer, just in case, usleep sleeps for longer
//than that because it is prone to doing so (that's the whole reason
//why we have ft_usleep). So if it ends up sleeping for 59,000 microseconds
//for example, then it is fine. But if I hadn't split this period into
//a quarter, then I would have ended up calling usleep on 200,000microseconds
//my original wait time, and so, if usleep exceeds that and ends up
//waiting for 209,000 microseconds for example, then that beat the whole
//point of creating an ft_usleep. So back to 59,000 microseconds, on the next
//iteration, the elapsed time would have been 59,000 microseconds so now,
//since our remainder (141000 microseconds) is still larger than 1
//millisecond (or 1000 microseconds) we again use usleep to wait for 35250 
//microseconds (141000 / 4). Let's say it waited for a bit longer, who cares,
//so it waited for 35275 microseconds; then our total elapsed time thus far
//is 94275 microseconds (59000 + 35275). This keeps happening till our remainig
//time equates to 1000 microseconds or less. In that case, we stop using usleep
//so that we can catch it exactly before it hits the usec time we passed as an
//argument (which is precisely how long I want to wait for). At this point, we
//stop using usleep and we begin to use the spinlock to actually wait until we
//reach usec (but techincally msec which is just usec converted to millseconds).
//Now technically, we could have just done the spinlock loop:
//	while (get_time() - program->start_time < msec)
//		;
//from the get-go without this whole song and dance. But the thing is, spinlocks
//are not efficient, they consume CPU resources, aka they are CPU-intensive
//while the usage of the actual function usleep isn't. Usleep utilizes the
//underlying system's timer facilities to suspend the execution of the calling
//thread for the specified duration. It doesn't typically use a busy-wait loop,
//which would consume CPU resources unnecessarily. But since we cannot tweak it
//to bypass its inacurracy, we instead use it only up until a certain threshold
//(1000 microseconds remaining in this case) and then, we use a spinlock. Since
//spinlocks consume CPU resources, we opt to wait utilize it only in the last
//1000 microseconds of waiting, so from 199,000 microseconds up until
//200,000 microseconds, to limit the time the CPU is spent in this inefficient
//busy-wait loop. So this design limits the time the CPU is spent busy waiting
//inefficiently in the spinlock by utilizing the actual more effcieint usleep
//for the most part and only uses the actual spinlock in the last 1000
//microsecond time frame.
void	ft_usleep(size_t usec, t_program *program)
{
	size_t	msec;
	size_t	start;
	int		elapsed;
	int		remaining;

	(void) program;
	start = get_time();
	msec = usec / 1000;
	while (get_time() - start < msec)
	{
		elapsed = get_time() - start;
		remaining = msec - elapsed;
		if (remaining >= 1)
			usleep((remaining * 1e3) / 4);
		else
		{
			while (get_time() - start < msec)
				;
		}
	}
}

// void	ft_usleep(size_t usec, t_program *program)
// {
// 	size_t	start;
// 	size_t	msec;

// 	(void) program;
// 	msec = usec / 1000;
// 	start = get_time();
// 	while ((get_time() - start) < msec)
// 		usleep(500);
// }
