/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:49:18 by pscala            #+#    #+#             */
/*   Updated: 2024/06/11 19:46:07 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	del_tenv2(t_env **env)
{
	t_env	*current;

	current = *env;
	*env = (*env)->prev;
	(*env)->next = NULL;
	free(current->line);
	free(current);
}

void	del_tenv(t_env **env, t_var *var)
{
	t_env	*current;
	t_env	*tmp;

	current = *env;
	*env = (*env)->next;
	if (current->prev == NULL)
	{
		tmp = NULL;
		var->env = *env;
		printf("env->line %s\n", (*env)->line);
	}
	else
	{
		tmp = current->prev;
		tmp->next = (*env);
	}
	(*env)->prev = tmp;
	free(current->line);
	free(current);
}

t_env	*ft_unset2(t_env *current, t_env *tmp, t_var *vars)
{
	if (current->prev == NULL && current->next == NULL)
		(free(current->line), free(current), vars->env = NULL);
	else if (current->next == NULL)
	{
		tmp = current->prev;
		del_tenv2(&current);
	}
	else
	{
		tmp = current->prev;
		del_tenv(&current, vars);
	}
	return (tmp);
}

void	ft_unset(t_env **env, t_var *vars, char *var)
{
	t_env	*current;
	t_env	*tmp;
	int		len;
	int		flag;

	flag = 0;
	current = *env;
	len = ft_strlen(var);
	if (!current)
		return ;
	while (current)
	{
		tmp = NULL;
		if ((ft_strncmp(var, current->line, len) == 0
				&& current->line[len] == '=') || (ft_strncmp(var, current->line,
					len) == 0 && current->line[len] == '\0'))
		{
			flag = 1;
			tmp = ft_unset2(current, tmp, vars);
		}
		if (flag == 1)
			current = tmp;
		else
			current = current->next;
	}
}

int	unset(char **cmd, t_var *var)
{
	int	i;

	i = 1;
	if (!cmd[i])
		return (0);
	while (cmd[i])
	{
		ft_unset(&var->env, var, cmd[i]);
		i++;
	}
	return (0);
}
