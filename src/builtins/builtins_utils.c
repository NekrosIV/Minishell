/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:28:47 by kasingh           #+#    #+#             */
/*   Updated: 2024/06/08 19:26:42 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_dup_in_builtins(t_word *tmp)
{
	int	fd_in;

	fd_in = open(tmp->word, O_RDONLY);
	if (fd_in == -1)
	{
		perror(tmp->word);
		return (EXIT_FAILURE);
	}
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		perror("dup2(fd_in)");
		close(fd_in);
		return (EXIT_FAILURE);
	}
	close(fd_in);
	return (EXIT_SUCCESS);
}

int	do_dup_out_builtins(t_word *tmp)
{
	int	fd_out;

	if (tmp->token == REDIR_OUT)
		fd_out = open(tmp->word, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd_out = open(tmp->word, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out == -1)
	{
		perror(tmp->word);
		return (EXIT_FAILURE);
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		perror("dup2(fd_out)");
		close(fd_out);
		return (EXIT_FAILURE);
	}
	close(fd_out);
	return (EXIT_SUCCESS);
}

int	do_dup_builtins(t_word *tmp, int *old_fd_in, int *old_fd_out)
{
	int	error;

	(*old_fd_in) = dup(STDIN_FILENO);
	if ((*old_fd_in) == -1)
	{
		perror("dup()");
		return (EXIT_FAILURE);
	}
	(*old_fd_out) = dup(STDOUT_FILENO);
	if ((*old_fd_out) == -1)
	{
		perror("dup()");
		return (EXIT_FAILURE);
	}
	while (is_cmd(tmp->token) == 1 || tmp->token == SPACES)
	{
		if (tmp->token == REDIR_IN || tmp->token == HERE_DOC)
			error = do_dup_in_builtins(tmp);
		else if (tmp->token == REDIR_OUT || tmp->token == REDIR_APPEND)
			error = do_dup_out_builtins(tmp);
		if (error == 1)
			return (EXIT_FAILURE);
		tmp = tmp->next;
	}
	return (EXIT_SUCCESS);
}

int	restore_fds(int old_fd_in, int old_fd_out)
{
	if (old_fd_in != -1 || old_fd_out != -1)
	{
		if (dup2(old_fd_in, STDIN_FILENO) == -1)
		{
			perror("dup2()");
			return (EXIT_FAILURE);
		}
		close(old_fd_in);
		if (dup2(old_fd_out, STDOUT_FILENO) == -1)
		{
			perror("dup2()");
			return (EXIT_FAILURE);
		}
		close(old_fd_out);
	}
	return (EXIT_SUCCESS);
}

int	is_dir_in_cmd(t_word *tmp)
{
	while (is_cmd(tmp->token) || tmp->token == SPACES)
	{
		if (tmp->token == REDIR_IN || tmp->token == HERE_DOC
			|| tmp->token == REDIR_OUT || tmp->token == REDIR_APPEND)
			return (1);
		tmp = tmp->next;
	}
	return (0);
}
