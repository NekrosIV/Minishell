/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   befor_exe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:57:37 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/23 12:21:23 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	loop_here_doc(char *eof, int fd)
{
	char	*line;

	while (1)
	{
		ft_putstr_fd("here_doc> ", 1);
		line = get_next_line(0);
		if (!line || ft_strncmp(line, eof, ft_strlen(line)) == 0)
		{
			if (!line)
			{
				ft_putstr_fd("\nhere-document delimited ", 2);
				ft_putstr_fd("by end-of-file (wanted `", 2);
				ft_putstr_fd(eof, 2);
				ft_putendl_fd("')", 2);
			}
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
}

int	here_doc(t_word *tmp, t_var *var)
{
	char		*eof;
	static int	i = 0;
	int			fd;
	char		*file_name;
	char		*nb;

	nb = ft_itoa(i++);
	if (!nb)
		return (free_error(var, E_Malloc, "nb", 1), -1);
	file_name = ft_strjoin("/tmp/here_doc_", nb);
	if (!file_name)
		return (free(nb), free_error(var, E_Malloc, "file_name", 1), -1);
	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (free(nb), free(file_name), perror("here_doc :"), -1);
	eof = ft_strjoin(tmp->word, "\n");
	if (!eof)
		return (free(nb), free_error(var, E_Malloc, "eof", 1), -1);
	loop_here_doc(eof, fd);
	free(tmp->word);
	tmp->word = file_name;
	return (free(eof), free(nb), 1);
}

void	do_here_doc(t_var *var)
{
	t_word	*tmp;

	tmp = var->lexer;
	while (tmp && var->error == false)
	{
		if (tmp->token == HERE_DOC)
		{
			if (here_doc(tmp, var) == -1)
				var->error = true;
		}
		tmp = tmp->next;
	}
}

void	do_dup_in(int pipe_fd[2], int c_fd, int i, t_var *var)
{
	int		fd_in;
	t_word	*tmp;
	int		flag;

	flag = 0;
	tmp = var->lexer;
	while (tmp->token != END && tmp->token != PIPE)
	{
		if (tmp->token == REDIR_IN || tmp->token == HERE_DOC)
		{
			fd_in = open(tmp->word, O_RDONLY);
			if (fd_in == -1)
			{
				perror(tmp->word);
				(close(pipe_fd[0]), close(pipe_fd[1]), close(c_fd));
				exit(EXIT_FAILURE);
			}
			if (dup2(fd_in, STDIN_FILENO) == -1)
			{
				perror(tmp->word);
				(close(pipe_fd[0]), close(pipe_fd[1]), close(c_fd),
					close(fd_in));
				exit(EXIT_FAILURE);
			}
			(close(fd_in), flag = 1);
		}
		tmp = tmp->next;
	}
	if (flag == 0 && i != 0)
	{
		if (dup2(c_fd, STDIN_FILENO) == -1)
		{
			perror("dup2(c_fd)");
			(close(pipe_fd[0]), close(pipe_fd[1]), close(c_fd));
			exit(EXIT_FAILURE);
		}
	}
}
void	do_dup_out(int pipe_fd[2], t_var *var)
{
	int		fd_out;
	t_word	*tmp;
	int		flag;

	flag = 0;
	tmp = var->lexer;
	while (tmp->token != END && tmp->token != PIPE)
	{
		if (tmp->token == REDIR_OUT || tmp->token == REDIR_APPEND)
		{
			if (tmp->token == REDIR_OUT)
				fd_out = open(tmp->word, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			else
				fd_out = open(tmp->word, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd_out == -1)
			{
				perror(tmp->word);
				(close(pipe_fd[0]), close(pipe_fd[1]));
				exit(EXIT_FAILURE);
			}
			if (dup2(fd_out, STDOUT_FILENO) == -1)
			{
				perror(tmp->word);
				(close(pipe_fd[0]), close(pipe_fd[1])), close(fd_out);
				exit(EXIT_FAILURE);
			}
			(close(fd_out), flag = 1);
		}
		tmp = tmp->next;
	}
	if (flag == 0 && tmp->token != END)
	{
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
		{
			perror("dup2(pipe_fd[1])");
			(close(pipe_fd[0]), close(pipe_fd[1]));
			exit(EXIT_FAILURE);
		}
	}
}

void	print_split(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		ft_putendl_fd(split[i], 2);
		i++;
	}
}

void	child(int c_fd, int pipe_fd[2], int i, t_var *var)
{
	do_dup_in(pipe_fd, c_fd, i, var);
	(close(c_fd), close(pipe_fd[0]));
	do_dup_out(pipe_fd, var);
	ft_putendl_fd("werqwerw", 2);
	close(pipe_fd[1]);
	print_split(split_cmd(var));
	exit(EXIT_SUCCESS);
	// excute(get_cmd(av), env);
}
static int	wait_for_child(pid_t pid)
{
	int		status[2];
	pid_t	r_waitpid;

	while (1)
	{
		r_waitpid = waitpid(-1, &status[0], 0);
		if (r_waitpid == -1)
			break ;
		if (r_waitpid == pid)
			status[1] = status[0];
	}
	if (WIFEXITED(status[1]))
		return (WEXITSTATUS(status[1]));
	else if (WIFSIGNALED(status[1]))
		return (WTERMSIG(status[1]) + 128);
	return (0);
}

int	fork_loop(t_var *var, int nb_cmd)
{
	int		i;
	int		pipe_fd[2];
	int		c_fd;
	pid_t	pid;

	i = 0;
	c_fd = 0;
	while (nb_cmd > i)
	{
		if (i != 0)
			c_fd = pipe_fd[0];
		if (pipe(pipe_fd) == -1)
			return (close_fd(c_fd, i), -1);
		pid = fork();
		if (pid == -1)
			return (close(pipe_fd[0]), close_fd(c_fd, i), close(pipe_fd[1]),
				-1);
		if (pid == 0)
			child(c_fd, pipe_fd, i, var);
		close(pipe_fd[1]);
		close_fd(c_fd, i);
		i++;
		del_cmd(&var->lexer);
	}
	return (wait_for_child(pid));
}

void	exe_cmd(t_var *var)
{
	int	nb_pipe;

	var->envp = split_env(var->env);
	if (!var->envp)
		free_error(var, E_Malloc, "env", 1);
	nb_pipe = count_node_token(var->lexer, PIPE);
	if (fork_loop(var, nb_pipe + 1) == -1)
		free_error(NULL, E_pipe, NULL, -99);
}

void	before_exe(t_var *var)
{
	if (node_cmp_token(var->lexer, HERE_DOC) == 1)
		do_here_doc(var);
	if (var->error == false)
		exe_cmd(var);
}
