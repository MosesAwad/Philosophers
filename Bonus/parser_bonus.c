/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawad <mawad@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 00:27:41 by mawad             #+#    #+#             */
/*   Updated: 2024/03/26 00:28:01 by mawad            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

static void	spcsign_loop(const char *nptr, int *i, int *negative_flag)
{
	while ((nptr[*i] >= 9 && nptr[*i] <= 13) || nptr[*i] == 32)
		(*i)++;
	if (nptr[*i] == '+' || nptr[*i] == '-')
	{
		if (nptr[*i] == '-')
			*negative_flag *= -1;
		(*i)++;
	}
}

static long long int	ft_atol(const char *nptr)
{
	int				i;
	int				negative_flag;
	int				digit;
	long long int	store;

	i = 0;
	negative_flag = 1;
	store = 0;
	spcsign_loop(nptr, &i, &negative_flag);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		digit = nptr[i] - 48;
		if ((store > (9223372036854775807 - digit) / 10) && negative_flag == 1)
			return (2147483648);
		if ((store > (9223372036854775807 - digit) / 10) && negative_flag == -1)
			return (-2147483649);
		store = store * 10 + digit;
		i++;
	}
	return ((store * negative_flag));
}

static void	custom_message(int i)
{
	if (i == 1)
		exit_err("Philo amnt must be a positive integer");
	if (i == 2)
		exit_err("Time to die must be a positive integer");
	if (i == 3)
		exit_err("Time to eat must be a positive integer");
	if (i == 4)
		exit_err("Time to sleep must be a positive integer");
	if (i == 5)
		exit_err("meal limit must be a positive integer");
}

static t_bool	only_digits(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+')
		i++;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

void	parser(char *argv[])
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (ft_atol(argv[i]) >= INT_MAX || ft_atol(argv[i]) < 0
			|| only_digits(argv[i]) == FALSE)
			custom_message(i);
		i++;
	}
	if (argv[5])
	{
		if (ft_atol(argv[5]) == 0)
			exit_err("Setting meal limit to 0"
				" does not result in a simulation\n");
	}
}
