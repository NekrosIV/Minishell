/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:49:02 by pscala            #+#    #+#             */
/*   Updated: 2024/06/04 16:20:16 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	go_to_digit(char *str, int *sign)
{
	int	i;

	i = 0;
	while (str[i] && (str[i] == ' ' || str[i] == '\t'))
		i++;
	if (str[i] == '-')
	{
		*sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	return (i);
}

bool	ft_strtoll(char *str, long long *exit)
{
	int							sign;
	unsigned long long			value;
	const unsigned long long	max_div_10 = LLONG_MAX / 10;
	int							d;
	int							i;

	sign = 1;
	value = 0;
	*exit = 0;
	i = go_to_digit(str, &sign);
	while (ft_isdigit(str[i]))
	{
		d = str[i] - '0';
		if (value > max_div_10 || (value == max_div_10 && d > (LLONG_MAX % 10)))
			return (false);
		value = value * 10 + d;
		i++;
	}
	if (str[i] && !ft_isdigit(str[i]))
		return (false);
	if ((sign == -1 && value > (unsigned long long)LLONG_MAX + 1))
		return (false);
	*exit = sign * value;
	return (true);
}

int	exit_builtin(char **cmd, t_var *var)
{
	long long	exit;
	bool		good_llint;

	exit = 0;
	var->exit = true;
	if (var->in_fork == false)
		ft_putstr_fd("exit\n", 1);
	if (cmd[1])
	{
		good_llint = ft_strtoll(cmd[1], &exit);
		if (cmd[2] && good_llint == true)
			return (free_error(NULL, E_EXIT, E_EXIT_MANY, -2), 2);
		else
		{
			if (good_llint == true)
				return (exit % 256);
			else
			{
				ft_putstr_fd(E_EXIT, 2);
				return (free_error(NULL, cmd[1], E_EXIT_NUM, -2), 2);
			}
		}
	}
	return (g_exit_status);
}
