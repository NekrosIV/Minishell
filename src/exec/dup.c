/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dup.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 16:47:17 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/14 17:35:47 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_dup_in2(int pipe_fd[2], int c_fd, t_var *var)
{
	if (dup2(c_fd, STDIN_FILENO) == -1)
	{
		perror("dup2(c_fd)");
		(close(pipe_fd[0]), close(pipe_fd[1]), close(c_fd));
		free_error(var, NULL, NULL, EXIT_FAILURE);
	}
}

void	do_dup_in(int pipe_fd[2], int flag[4], t_word *tmp, t_var *var)
{
	int	fd_in;

	if (tmp->token == REDIR_IN || tmp->token == HERE_DOC)
	{
		fd_in = open(tmp->word, O_RDONLY);
		if (fd_in == -1)
		{
			perror(tmp->word);
			(close(pipe_fd[0]), close(pipe_fd[1]), close(flag[3]));
			free_error(var, NULL, NULL, EXIT_FAILURE);
		}
		if (dup2(fd_in, STDIN_FILENO) == -1)
		{
			perror("dup2(fd_in)");
			(close(pipe_fd[0]), close(pipe_fd[1]), close(flag[3]),
				close(fd_in));
			free_error(var, NULL, NULL, EXIT_FAILURE);
		}
		close(fd_in);
	}
	else if (flag[0] != 0 && flag[2] != 0)
		do_dup_in2(pipe_fd, flag[3], var);
}

void	do_dup_out2(int pipe_fd[2], t_word *tmp, t_var *var)
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
		free_error(var, NULL, NULL, EXIT_FAILURE);
	}
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		perror("dup2(fd_out)");
		(close(pipe_fd[0]), close(pipe_fd[1])), close(fd_out);
		free_error(var, NULL, NULL, EXIT_FAILURE);
	}
	close(fd_out);
}

void	do_dup_out(int pipe_fd[2], int flag[4], t_word *tmp, t_var *var)
{
	if (tmp->token == REDIR_OUT || tmp->token == REDIR_APPEND)
		do_dup_out2(pipe_fd, tmp, var);
	else if (flag[1] != 0 && tmp->token != END)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2(pipe_fd[1])");
			(close(pipe_fd[0]), close(pipe_fd[1]));
			free_error(var, NULL, NULL, EXIT_FAILURE);
		}
	}
}

void	do_dup(int c_fd, int pipe_fd[2], int i, t_var *var)
{
	t_word	*tmp;
	int		flag[4];

	tmp = var->lexer;
	flag[0] = 1;
	flag[1] = 1;
	flag[2] = i;
	flag[3] = c_fd;
	if (tmp == NULL)
	{
		ft_printf("Error: lexer is NULL\n");
		return ;
	}
	while (tmp && tmp->token != END && tmp->token != PIPE)
	{
		if (tmp->token == REDIR_IN || tmp->token == HERE_DOC)
		{
			do_dup_in(pipe_fd, flag, tmp, var);
			flag[0] = 0;
		}
		else if (tmp->token == REDIR_OUT || tmp->token == REDIR_APPEND)
		{
			do_dup_out(pipe_fd, flag, tmp, var);
			flag[1] = 0;
		}
		tmp = tmp->next;
	}
	(do_dup_in(pipe_fd, flag, tmp, var), do_dup_out(pipe_fd, flag, tmp, var));
	(close_fd(c_fd, i), close(pipe_fd[0]), close(pipe_fd[1]));
}
