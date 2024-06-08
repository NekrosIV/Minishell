/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:49:11 by pscala            #+#    #+#             */
/*   Updated: 2024/06/08 19:50:54 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(char **cmd, t_var *var)
{
	t_env	*env;

	if (cmd[1])
		return (error_msg_builtins(E_ENV, cmd[1], "No such file or directory"),
			1);
	env = var->env;
	while (env)
	{
		if (ft_strchr(env->line, '=') != NULL)
			ft_putendl_fd(env->line, 1);
		env = env->next;
	}
	return (0);
}
