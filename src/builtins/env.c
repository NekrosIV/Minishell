/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:49:11 by pscala            #+#    #+#             */
/*   Updated: 2024/06/06 18:36:19 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(char **cmd, t_var *var)
{
	t_env	*env;

	env = var->env;
	while (env)
	{
		if (ft_strchr(env->line, '=') != NULL)
			ft_putendl_fd(env->line, 1);
		env = env->next;
	}
	return (0);
}
