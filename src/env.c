/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:02:03 by pscala            #+#    #+#             */
/*   Updated: 2024/04/16 16:16:38 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_list_env(t_env **env)
{
	t_env	*temp;
	t_env	*current;

	if (*env)
	{
		current = *env;
		while (current)
		{
			temp = current->next;
			free(current->line);
			free(current);
			current = temp;
		}
		*env = NULL;
	}
}

void	add_node_env(t_env **envs, char *str)
{
	t_env	*new_env;
	t_env	*last_env;

	new_env = malloc(sizeof(t_env));
	if (!new_env)
	{
		printf("Error: malloc failed\n");
		exit(1);
	}
	new_env->line = str;
	new_env->next = NULL;
	last_env = NULL;
	if (!(*envs))
	{
		*envs = new_env;
		new_env->prev = NULL;
	}
	else
	{
		last_env = get_last_tenv(*envs);
		last_env->next = new_env;
		new_env->prev = last_env;
	}
}

void	init_env(t_env **envs, char **env)
{
	int i;

	i = 0;
	if (!env || !env[0])
		envs = NULL;
	else
	{
		while (env[i])
		{
			add_node_env(envs, ft_strdup(env[i]));
			i++;
		}
	}
}