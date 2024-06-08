/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:49:02 by pscala            #+#    #+#             */
/*   Updated: 2024/06/08 19:34:36 by pscala           ###   ########.fr       */
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

bool	ft_strtoll(char *str, unsigned long long *exit)
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
	if (((str[i] && !ft_isdigit(str[i])) || str[0] == '\0'))
		return (false);
	if ((sign == -1 && value > (unsigned long long)LLONG_MAX + 1))
		return (false);
	*exit = sign * value;
	return (true);
}

int	exit_builtin(char **cmd, t_var *var)
{
	unsigned long long	exit;
	bool				good_llint;

	exit = 0;
	if (var->in_fork == false && isatty(0))
		ft_putstr_fd("exit\n", 1);
	if (cmd[1])
	{
		good_llint = ft_strtoll(cmd[1], &exit);
		if (cmd[2] && good_llint == true)
			return (ft_putstr_fd(E_EXIT E_EXIT_MANY, 2), 1);
		else
		{
			var->exit = true;
			if (good_llint == true)
				return (exit % 256);
			else
			{
				(ft_putstr_fd(E_EXIT, 2), ft_putstr_fd(cmd[1], 2));
				return (ft_putstr_fd(": ", 2), ft_putstr_fd(E_EXIT_NUM, 2), 2);
			}
		}
	}
	var->exit = true;
	return (g_exit_status);
}
