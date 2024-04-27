/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 16:44:47 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/27 18:56:32 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec(char **cmd, char **env)
{
	int		i;
	char	*path;
	char	**tmp_path;

	tmp_path = NULL;
	path = NULL;
	i = 0;
	if (access(cmd[0], F_OK) == 0)
		path = ft_strdup(cmd[0]);
	else if (env[i])
	{
		while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
			i++;
		if (env[i])
			tmp_path = ft_split(&env[i][5], ':');
		if (!tmp_path)
			error_msg(path, cmd, env);
		path = get_path(cmd, tmp_path);
	}
	if (!path)
		error_msg(path, cmd, env);
	execve(path, cmd, env);
	error_msg(path, cmd, env);
}

void	child(int c_fd, int pipe_fd[2], int i, t_var *var)
{
	char	**env;
	char	**cmd;

	do_dup(c_fd, pipe_fd, i, var);
	if (node_cmp_token(var->lexer, CMD) == 0 && node_cmp_token(var->lexer,
			QUOTE_CMD) == 0)
		free_error(var, NULL, NULL, 0);
	env = split_env(var->env);
	if (!env)
		free_error(var, E_Malloc, "env", 1);
	cmd = split_cmd(var);
	if (!cmd)
		free_error(var, E_Malloc, "cmd", 1);
	var->exit = true;
	free_var(var);
	if (cmd[0][0] == '\0')
	{
		ft_putstr_fd("command not found: ", 2);
		ft_putendl_fd(cmd[0], 2);
		free_split(env);
		free_split(cmd);
		exit(127);
	}
	exec(cmd, env);
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
	var->status = wait_for_child(pid);
	return (close(pipe_fd[0]), 0);
}

void	exe_cmd(t_var *var)
{
	int		nb_pipe;
	t_word	*tmp;

	nb_pipe = 0;
	tmp = var->lexer;
	while (tmp)
	{
		if (tmp->token == PIPE)
			nb_pipe++;
		tmp = tmp->next;
	}
	if (fork_loop(var, nb_pipe + 1) == -1)
		free_error(NULL, E_pipe, NULL, -99);
}
