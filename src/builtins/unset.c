/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:49:18 by pscala            #+#    #+#             */
/*   Updated: 2024/06/07 18:33:55 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void del_tenv2(t_env **env)
{
	t_env	*current;

	current = *env;
	*env = (*env)->prev;
	(*env)->next = NULL;
	free(current->line);
	free(current);
}

void del_tenv(t_env **env)
{
	t_env	*current;
	t_env	*tmp;

	current = *env;
	*env = (*env)->next;
	if (current->prev == NULL)
		tmp = NULL;
	else
	{
		tmp = current->prev;
		tmp->next = (*env);
	}
	(*env)->prev = tmp;
	free(current->line);
	free(current);
}


void ft_unset(t_env **env, char *var)
{
	t_env *current;
	t_env *tmp;
	int		len;
	int flag;
	
	flag = 0;
	current = *env;
	len = ft_strlen(var);
	if (!current)
		return;
	ft_putstr_fd("variable que l'on veut supprimer: ", 1);
	ft_putendl_fd(var, 1);
	while (current)
	{
		tmp = NULL;
		if ((ft_strncmp(var, current->line, len) == 0 && current->line[len] == '=') 
			|| (ft_strncmp(var, current->line, len) == 0 && current->line[len] == '\0'))
		{
			flag = 1;
			if (current->prev == NULL && current->next == NULL)
				(free(current->line), free(current));
			else if (current->next == NULL)
			{
				tmp = current->prev;
				del_tenv2(&current);
				ft_putstr_fd(var, 1);
				ft_putstr_fd(" a été supprimée avec douceur\n", 1);
				ft_putstr_fd("\n", 1);

			}
			else 
			{
				tmp = current->prev;
				del_tenv(&current);
				ft_putstr_fd(var, 1);
				ft_putstr_fd(" a été supprimée avec gentillesse\n", 1);
				ft_putstr_fd("\n", 1);

			}	
		}
		if (flag == 1)
			current = tmp;	
		else 
		{
			current = current->next;
			// ft_putstr_fd("rien a supprimer...\n", 1);
		}
	}
}

int	unset(char **cmd, t_var *var)
{
	int i;

	i = 1;
	if (!cmd[i])
		return (free_error(NULL, "unset: ", "not enough arguments", -1), 1);
	while (cmd[i])
	{
		ft_unset(&(var->env), cmd[i]);
		i++;
	}
	return (0);
}
