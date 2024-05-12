/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 16:25:15 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/12 15:50:18 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_exit(t_var *var)
{
	if (!var->line || !ft_strncmp(var->line, "exit", 4))
	{
		if (var->line)
			ft_putstr_fd("exit\n", 1);
		free_error(var, NULL, NULL, g_exit_status);
	}
	add_history(var->line);
}
