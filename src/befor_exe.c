/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   befor_exe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:57:37 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/21 18:49:21 by kasingh          ###   ########.fr       */
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

int	here_doc(t_word *tmp)
{
	char		*eof;
	static int	i = 0;
	int			fd;
	char		*file_name;
	char		*nb;

	nb = ft_itoa(i++);
	if (!nb)
		return (free_error(NULL, E_Malloc, "nb", 1), -1);
	file_name = ft_strjoin("/tmp/here_doc", nb);
	if (!file_name)
		return (free(nb), free_error(NULL, E_Malloc, "file_name", 1), -1);
	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (free(nb), perror("here_doc :"), -1);
	eof = ft_strjoin(tmp->word, "\n");
	if (!eof)
		return (free(nb), free_error(NULL, E_Malloc, "eof", 1), -1);
	loop_here_doc(eof, fd);
	free(tmp->word);
	tmp->word = file_name;
	return (free(eof), free(nb), 1);
}

void	do_here_doc(t_var *var)
{
	t_word	*tmp;

	tmp = var->lexer;
	while (tmp && var->exit == false)
	{
		if (tmp->token == HERE_DOC)
		{
			if (here_doc(tmp) == -1)
				var->exit = true;
		}
		tmp = tmp->next;
	}
}

void	child_loop(int c_fd, int pipe_fd[2], char *av, char **env)
{
	close(pipe_fd[0]);
	if (dup2(c_fd, STDIN_FILENO) == -1)
	{
		perror("child_loop:dup2(c_fd)");
		close(c_fd);
		exit(EXIT_FAILURE);
	}
	close(c_fd);
	if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
	{
		perror("child_loop:dup2(pipe_fd[1])");
		close(pipe_fd[1]);
		exit(EXIT_FAILURE);
	}
	close(pipe_fd[1]);
	excute(get_cmd(av), env);
}

int	fork_loop(t_var *var, int nb_cmd)
{
	int		i;
	int		pipe_fd[2];
	int		c_fd;
	pid_t	pid;

	i = 0;
	while (nb_cmd > i)
	{
		if (i != 0)
			c_fd = pipe_fd[0];
		if (pipe(pipe_fd) == -1)
			return (close(c_fd), -1);
		pid = fork();
		if (pid == -1)
			return (close(pipe_fd[0]), close(c_fd), close(pipe_fd[1]), -1);
		if (pid == 0)
			child(c_fd, pipe_fd, i, var);
		close(pipe_fd[1]);
		if (i != 0)
			close(c_fd);
		i++;
	}
	return (0);
}

char	**split_env(t_env *env)
{
	char	**result;
	int		i;

	result = malloc(sizeof(char *) * (count_node_env(env) + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (env)
	{
		result[i] = ft_strdup(env->line);
		if (!result[i])
			return (free_tab(result), NULL);
		i++;
		env = env->next;
	}
	result[i] = NULL;
	return (result);
}

void	exe_cmd(t_var *var)
{
	char	**env;
	int		nb_pipe;

	env = split_env(var->env);
	if (!env)
		free_error(NULL, E_Malloc, "env", 1);
	nb_pipe = count_node_token(var->lexer, PIPE);
	if (fork_loop(var, nb_pipe) == -1)
		free_error(NULL, E_pipe, NULL, -99);
}

void	before_exe(t_var *var)
{
	if (node_cmp_token(var->lexer, HERE_DOC) == 1)
		do_here_doc(var);
	if (var->exit == false)
		exe_cmd(var);
}
