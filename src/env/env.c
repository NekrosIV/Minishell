/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:02:03 by pscala            #+#    #+#             */
/*   Updated: 2024/05/17 13:55:38 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	**split_env(t_env *env)
{
	char	**result;
	int		i;

	result = malloc(sizeof(char *) * (count_node_env(env) + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (env)
	{
		result[i] = ft_strdup(env->line);
		if (!result[i])
			return (free_split(result), NULL);
		i++;
		env = env->next;
	}
	result[i] = NULL;
	return (result);
}

void	add_pwd(t_env **envs)
{
	char	*pwd;
	char	*line;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		printf("Error: getcwd failed\n");
		exit(1);
	}
	line = ft_strjoin("PWD=", pwd);
	if (!line)
	{
		printf("Error: ft_strjoin failed\n");
		exit(1);
	}
	add_node_env(envs, line);
	free(pwd);
}

void	init_env(t_env **envs, char **env)
{
	int	i;

	i = 0;
	if (!env || !env[0])
		add_pwd(envs);
	else
	{
		while (env[i])
		{
			add_node_env(envs, ft_strdup(env[i]));
			i++;
		}
	}
}
