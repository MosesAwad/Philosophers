/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 00:17:56 by mawad             #+#    #+#             */
/*   Updated: 2024/03/28 23:05:20 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <sys/time.h>
# include <errno.h>
# include <stdio.h>
# include <unistd.h>
# include <pthread.h>
# include <stdlib.h>
# include <limits.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <semaphore.h>
# include <errno.h>
# include <signal.h>
# include <sys/wait.h>

typedef struct s_program	t_program;

typedef enum s_bool
{
	FALSE = 0,
	TRUE = 1
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

typedef enum s_sem_ops
{
	OPEN,
	CLOSE,
	POST,
	WAIT,
	UNLINK
}	t_sem_ops;

typedef enum s_philo_status
{
	DEAD,
	FIRST_FORK,
	SECOND_FORK,
	EATING,
	SLEEPING,
	THINKING,
}	t_philo_status;

// typedef struct s_fork
// {
// 	int				fork_id;
// 	pthread_mutex_t	fork;
// }	t_fork;

typedef struct s_philo
{
	int				philo_id;
	int				meal_count;
	size_t			last_meal_time;
	t_bool			philo_full;
	pthread_t		monitor;
	sem_t			*read_sem;
	sem_t			*end_sem;
	t_program		*program;
}		t_philo;

//lmt stands for last meal time
typedef struct s_program
{
	int				philo_amnt;
	int				meal_limit;
	t_bool			game_over;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			start_time;
	sem_t			*forks;
	pid_t			*philos;
	sem_t			*write_sem;
	sem_t			*balance_sem;
	sem_t			*kill_sem;
	t_philo			philo_data;
}	t_program;

# define OG		"\033[0m"
# define RED	"\033[1;31m"
# define GRN	"\033[1;32m"
# define YLW	"\033[1;33m"
# define BLU	"\033[1;34m"
# define PRL	"\033[0;35m"

//actions_bonus.c
void	write_status(t_program *program, t_philo_status status);
void	ft_eat(t_program *program);
void	ft_sleep(t_program *program);
void	ft_think(t_program *program);

//ft_itoa_bonus.c
char	*ft_itoa(int n);

//handlers_bonus.c
void	handle_threads(pthread_t *thread, void *(*routine)(void *),
			void *arg, t_thread_ops operation);
sem_t	*handle_semaphores(sem_t *sem, const char *name,
			unsigned int value, t_sem_ops operation);

//inits_bonus.c
void	init_program(t_program *program, char *argv[]);

//monitor_bonus.c
void	*monitor_simulation(void *arg);

//parser_bonus.c
void	parser(char *argv[]);

//philo_utils_bonus.c
void	exit_err(char *message);
void	*ft_alloc(size_t size);
int		ft_atoi(char *str);
void	ft_destroy(t_program *program);

//philo_utils_bonus2.c
size_t	get_time(void);
void	ft_usleep(size_t usec, t_program *program);

//philo_utils_bonus3.c
size_t	ft_strlen(const char *s);
char	*ft_strdup(const char *s);
char	*ft_strjoin(char *s1, const char *s2);

//read_write_bonus.c
t_bool	game_over(t_program *program);
void	bool_writer(sem_t *semaphore, t_bool *dest, t_bool new_value);

//simulation_bonus.c
void	set_up_procs(t_program *program);

#endif