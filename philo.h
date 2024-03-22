/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/11 23:31:18 by mawad             #+#    #+#             */
/*   Updated: 2024/03/23 00:17:52 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <sys/time.h>
# include <errno.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <stdlib.h>

typedef struct s_program	t_program;

typedef enum s_bool
{
	false = 0,
	true = 1
}	t_bool;

typedef enum s_thread_ops
{
	CREATE,
	JOIN
}	t_thread_ops;

typedef enum s_mutex_ops
{
	INIT,
	LOCK,
	UNLOCK,
	DESTROY
}	t_mutex_ops;

typedef enum s_philo_status
{
	DEAD,
	FIRST_FORK,
	SECOND_FORK,
	EATING,
	SLEEPING,
	THINKING,
}	t_philo_status;

typedef struct s_fork
{
	int				fork_id;
	pthread_mutex_t	fork;
}	t_fork;

typedef struct s_philo
{
	int				philo_id;
	int				meal_count;
	size_t			last_meal_time;
	t_bool			philo_full;
	pthread_t		thread;
	t_fork			*left_fork;
	t_fork			*right_fork;
	t_program		*program;
	pthread_mutex_t	philo_mutex;
}		t_philo;

//lmt stands for last meal time
typedef struct s_program
{
	int				philo_amnt;
	int				meal_limit;
	t_bool			all_threads_ready;
	t_bool			game_over;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			lmt_init_count;
	size_t			start_time;
	t_fork			*forks;
	t_philo			*philos;
	pthread_t		monitor;
	pthread_mutex_t	read_mutex;
	pthread_mutex_t	write_mutex;
}	t_program;

# define OG		"\033[0m"
# define RED	"\033[1;31m"
# define GRN	"\033[1;32m"
# define YLW	"\033[1;33m"
# define BLU	"\033[1;34m"
# define PRL	"\033[0;35m"

//main.c

//actions.c
void	write_details(t_philo *philo, t_philo_status status);
void	write_status(t_philo *philo, t_philo_status status);
void	ft_eat(t_philo *philo);
void	ft_sleep(t_philo *philo);
void	ft_think(t_philo *philo);

//handlers.c
void	handle_threads(pthread_t *thread, void *(*routine)(void *),
			void *arg, t_thread_ops operation);
void	handle_mutexes(pthread_mutex_t *mutex, t_mutex_ops operation);

//inits.c
void	init_program(t_program *program, char *argv[]);

//monitor.c
void	*monitor_simulation(void *arg);

//philo_utils.c
void	exit_err(char *message);
void	*ft_alloc(size_t size);
int		ft_atoi(char *str);

//philo_utils2.c
t_bool	game_over(t_program *program);
t_bool	check_full(t_philo *philo, t_program *program);
void	ft_usleep(size_t usec, t_program *program);
size_t	get_time(void);

//read_write.c
t_bool	bool_reader(pthread_mutex_t *mutex, t_bool *read);
void	bool_writer(pthread_mutex_t *mutex, t_bool *dest, t_bool new_value);
size_t	st_reader(pthread_mutex_t *mutex, size_t *read);
void	st_writer(pthread_mutex_t *mutex, size_t *dest, size_t new_value);

//simulation.c
void	dinner_commence(t_program *program);

#endif