/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 13:44:12 by kasingh           #+#    #+#             */
/*   Updated: 2024/06/08 20:32:01 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	can_i_run_without_fork(t_var *var, t_word *tmp, t_word *head, int i)
{
	if (var->in_parenth == false && head->prev == NULL && i != 0)
		return (0);
	if (var->execute_next && is_cmd(tmp->token) == 1 && is_builtins(tmp) != 0)
	{
		while (is_cmd(tmp->token) == 1 || tmp->token == SPACES)
			tmp = tmp->next;
		if (tmp->token != PIPE)
			return (1);
	}
	return (0);
}

void	run_without_fork(t_var *var, t_word **head, int c_fd, int pipe_fd[2])
{
	t_word	*tmp;

	tmp = var->lexer;
	var->lexer = *head;
	g_exit_status = do_bultins(var);
	var->lexer = tmp;
	if (var->exit == true)
	{
		if (c_fd >= 0)
			close(c_fd);
		(close(pipe_fd[0]), close(pipe_fd[1]));
		free_error(var, NULL, NULL, g_exit_status);
	}
}

bool	run_in_fork(t_var *var, t_word *tmp, bool flag)
{
	return (var->execute_next && (is_cmd(tmp->token) == 1
			|| tmp->token == PARENTH_OPEN) && flag == true);
}

int	do_bonus_cmd(int c_fd, int pipe_fd[2], int i, t_var *var)
{
	pid_t	pid;
	t_word	*tmp;
	t_word	*h;
	bool	flag;

	tmp = var->lexer;
	h = tmp;
	flag = true;
	while (tmp != NULL && tmp->token != END && tmp->token != PIPE)
	{
		if (can_i_run_without_fork(var, tmp, h, i) && flag == true)
			(run_without_fork(var, &h, c_fd, pipe_fd), flag = false);
		else if (run_in_fork(var, tmp, flag))
		{
			pid = fork();
			if (pid == -1)
				return (close_all_fd(pipe_fd, c_fd, i, true), -1);
			if (pid == 0)
				(new_lst(&h, &var->lexer, tmp), child(c_fd, pipe_fd, i, var));
			(need_to_wait(tmp, pid), flag = false);
		}
		flag = update_execution_state(&h, tmp, var, flag);
		tmp = update_tmp(tmp, var);
	}
	return (0);
}
