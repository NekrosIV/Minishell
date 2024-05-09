/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 16:47:17 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/09 16:13:42 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	do_dup_in2(int pipe_fd[2], int c_fd, t_word *tmp)
{
	if (dup2(c_fd, STDIN_FILENO) == -1)
	{
		perror("dup2(c_fd)");
		(close(pipe_fd[0]), close(pipe_fd[1]), close(c_fd));
		return (-1);
	}
	return (0);
}

int	do_dup_in(int pipe_fd[2], int c_fd, int flag[3], t_word *tmp)
{
	int	fd_in;

	if (tmp->token == REDIR_IN || tmp->token == HERE_DOC)
	{
		fd_in = open(tmp->word, O_RDONLY);
		if (fd_in == -1)
		{
			perror(tmp->word);
			(close(pipe_fd[0]), close(pipe_fd[1]), close(c_fd));
			return (-1);
		}
		if (dup2(fd_in, STDIN_FILENO) == -1)
		{
			perror("dup2(fd_in)");
			(close(pipe_fd[0]), close(pipe_fd[1]), close(c_fd), close(fd_in));
			return (-1);
		}
		close(fd_in);
	}
	else if (flag[0] != 0 && flag[2] != 0)
		return (do_dup_in2(pipe_fd, c_fd, tmp));
	return (0);
}

int	do_dup_out2(int pipe_fd[2], t_word *tmp)
{
	int	fd_out;

	if (tmp->token == REDIR_OUT)
		fd_out = open(tmp->word, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else
		fd_out = open(tmp->word, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd_out == -1)
	{
		perror(tmp->word);
		(close(pipe_fd[0]), close(pipe_fd[1]));
		return (-1);
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		perror("dup2(fd_out)");
		(close(pipe_fd[0]), close(pipe_fd[1])), close(fd_out);
		return (-1);
	}
	close(fd_out);
	return (0);
}

int	do_dup_out(int pipe_fd[2], int flag[3], t_word *tmp)
{
	if (tmp->token == REDIR_OUT || tmp->token == REDIR_APPEND)
		return (do_dup_out2(pipe_fd, tmp));
	else if (flag[1] != 0 && tmp->token != END)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2(pipe_fd[1])");
			(close(pipe_fd[0]), close(pipe_fd[1]));
			return (-1);
		}
	}
	return (0);
}

void	do_dup(int c_fd, int pipe_fd[2], int i, t_var *var)
{
	t_word	*tmp;
	int		flag[3];

	(tmp = var->lexer, flag[0] = 1, flag[1] = 1, flag[2] = i);
	while (tmp->token != END && tmp->token != PIPE)
	{
		if (tmp->token == REDIR_IN || tmp->token == HERE_DOC)
		{
			if (do_dup_in(pipe_fd, c_fd, flag, tmp) == -1)
				free_error(var, NULL, NULL, EXIT_FAILURE);
			flag[0] = 0;
		}
		else if (tmp->token == REDIR_OUT || tmp->token == REDIR_APPEND)
		{
			if (do_dup_out(pipe_fd, flag, tmp) == -1)
				free_error(var, NULL, NULL, EXIT_FAILURE);
			flag[1] = 0;
		}
		tmp = tmp->next;
	}
	if (do_dup_in(pipe_fd, c_fd, flag, tmp) == -1 || do_dup_out(pipe_fd, flag,
			tmp) == -1)
		free_error(var, NULL, NULL, EXIT_FAILURE);
	(close_fd(c_fd, i), close(pipe_fd[0]), close(pipe_fd[1]));
}
