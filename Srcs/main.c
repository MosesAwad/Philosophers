/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 22:38:43 by mawad             #+#    #+#             */
/*   Updated: 2024/03/24 01:01:58 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	dinner_commence(&program);
	ft_destroy(&program);
	return (0);
}

// int	main(int argc, char *argv[])
// {
// 	t_program	program;

// 	if (!(argc == 5 || argc == 6))
// 	{
// 		printf("Please insert 5 or 6 strict arguments only\n");
// 		return (1);
// 	}

// 	init_program(&program, argv);

// 	//debugagge - start
// 	printf("num of philos is %d\n", program.philo_amnt);
// 	printf("meal limit is %d\n", program.meal_limit);
// 	printf("time to die is %zu\n", program.time_to_die);
// 	printf("time to eat is %zu\n", program.time_to_eat);
// 	printf("time to sleep is %zu\n", program.time_to_sleep);
// 	for (int i = 0; i < program.philo_amnt; i++)
// 	{
// 		printf("-------------------\n");
// 		printf("philo id is %d\n", program.philos[i].philo_id);
// 		printf("philo left fork is fork #%d\n",
// 			program.philos[i].left_fork->fork_id);
// 		printf("philo right fork is fork #%d\n",
// 			program.philos[i].right_fork->fork_id);
// 		printf("--------------------\n");
// 	}
// 	//debugagge - end

// 	dinner_commence(&program);
// 	return (0);
// }
