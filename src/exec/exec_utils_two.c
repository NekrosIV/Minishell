/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_two.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:24:47 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/11 13:45:57 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	node_cmp_token(t_word *lexer, int token)
{
	while (lexer)
	{
		if (lexer->token == token)
			return (1);
		lexer = lexer->next;
	}
	return (0);
}

void	close_fd(int fd, int i)
{
	if (i != 0)
		close(fd);
}

void	close_all_fd(int pipe_fd[2], int c_fd, int i, bool close_fd_0)
{
	if (close_fd_0 == true)
		close(pipe_fd[0]);
	close_fd(c_fd, i);
	close(pipe_fd[1]);
}

int	do_simple_cmd(int c_fd, int pipe_fd[2], int i, t_var *var)
{
	var->last_pid = fork();
	if (var->last_pid == -1)
		return (close_all_fd(pipe_fd, c_fd, i, true), -1);
	if (var->last_pid == 0)
		child(c_fd, pipe_fd, i, var);
	var->bonus_cmd = false;
	return (0);
}

int	choose_how_to_exec(int c_fd, int pipe_fd[2], int i, t_var *var)
{
	if (is_bonus_cmd(var->lexer) == 1)
	{
		if (do_bonus_cmd(c_fd, pipe_fd, i, var) == -1)
			return (-1);
		var->bonus_cmd = true;
	}
	else if (do_simple_cmd(c_fd, pipe_fd, i, var) == -1)
		return (-1);
	return (0);
}
