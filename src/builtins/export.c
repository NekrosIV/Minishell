/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:49:15 by pscala            #+#    #+#             */
/*   Updated: 2024/06/06 18:35:07 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env	*find_smallest_node(t_env *head)
{
	t_env	*smallest;
	t_env	*current;

	smallest = head;
	current = head->next;
	while (current != NULL)
	{
		if (ft_strcmp(current->line, smallest->line) < 0)
			smallest = current;
		current = current->next;
	}
	return (smallest);
}

t_env	*find_next_smallest_node(t_env *head, t_env *current_smallest)
{
	t_env	*smallest;
	t_env	*current;

	smallest = NULL;
	current = head;
	while (current != NULL)
	{
		if (ft_strcmp(current_smallest->line, current->line) < 0)
		{
			if (smallest == NULL || ft_strcmp(current->line,
					smallest->line) < 0)
				smallest = current;
		}
		current = current->next;
	}
	return (smallest);
}

void	print_env_ordered(t_env *env_list)
{
	t_env	*smallest;

	if (env_list == NULL)
		return ;
	smallest = find_smallest_node(env_list);
	while (smallest != NULL)
	{
		ft_printf("declare -x %s\n", smallest->line);
		smallest = find_next_smallest_node(env_list, smallest);
	}
}

int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !ft_isalpha(str[0]))
		return (0);
	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}
void	add_variable_in_env(t_var *var, char *tmp)
{
	t_env	*env;

	env = var->env;
	while (env)
	{
		if (ft_strcmp(env->line, tmp) == 0)
		{
			free(env->line);
			env->line = tmp;
			return ;
		}
		env = env->next;
	}
	add_node_env(&var->env, tmp);
}

void	choose_how_to_add(t_var *var, char *cmd)
{
	char	*name_end;
	size_t	name_len;
	char	*name;
	char	*tmp;

	tmp = ft_strdup(cmd);
	if (!tmp)
		free_error(var, E_MALLOC, "export()", 1);
	name_end = ft_strchr(cmd, '=');
	if (name_end)
	{
		printf("in\n");
		name_len = ft_strlen(cmd) - ft_strlen(name_end);
		name = ft_strndup(cmd, name_len, 0);
		if (!name)
			free_error(var, E_MALLOC, "export()", 1);
		if (find_replace_env(var->env, name, name_len, tmp) == 1)
			add_node_env(&var->env, tmp);
		free(name);
	}
	else
		add_variable_in_env(var, tmp);
}

int	export(char **cmd, t_var *var)
{
	t_env	*env;
	int		i;
	int		exit;

	env = var->env;
	exit = 0;
	i = 1;
	if (!cmd[1])
		print_env_ordered(env);
	else
	{
		while (cmd[i])
		{
			if (is_valid_identifier(cmd[i]) == 0)
				(error_msg_builtins(E_EXPORT, cmd[i], E_EXPORT_ID), exit = 1);
			else
				choose_how_to_add(var, cmd[i]);
			i++;
		}
	}
	return (exit);
}
