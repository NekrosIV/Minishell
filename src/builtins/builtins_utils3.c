/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 19:25:13 by pscala            #+#    #+#             */
/*   Updated: 2024/06/08 20:05:04 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_bultins(t_var *var)
{
	char	**cmd;
	int		old_fd_in;
	int		old_fd_out;
	int		status_exit;
	t_word	*tmp;

	status_exit = 0;
	old_fd_in = -1;
	old_fd_out = -1;
	tmp = var->lexer;
	cmd = split_cmd(var);
	if (!cmd)
		free_error(var, E_MALLOC, "cmd", 1);
	if (var->in_fork == false && is_dir_in_cmd(tmp) == 1)
	{
		if (do_dup_builtins(tmp, &old_fd_in, &old_fd_out) == 1)
			return (restore_fds(old_fd_in, old_fd_out), free_split(cmd),
				EXIT_FAILURE);
	}
	status_exit = var->tab_builtins[is_builtins(cmd_found(var->lexer))](cmd,
			var);
	if (restore_fds(old_fd_in, old_fd_out))
		return (free_split(cmd), EXIT_FAILURE);
	return (free_split(cmd), status_exit);
}

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
