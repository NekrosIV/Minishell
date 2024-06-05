/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:49:11 by pscala            #+#    #+#             */
/*   Updated: 2024/06/05 16:47:40 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env(char **cmd, t_var *var)
{
	t_env	*env;

	env = var->env;
	while (env)
	{
		ft_putendl_fd(env->line, 1);
		env = env->next;
	}
	return (0);
}
