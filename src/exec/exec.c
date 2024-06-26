/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 16:44:47 by kasingh           #+#    #+#             */
/*   Updated: 2024/06/15 13:29:25 by kasingh          ###   ########.fr       */
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
	if (ft_strchr(cmd[0], '/') != NULL && access(cmd[0], F_OK) == 0)
		path = ft_strdup(cmd[0]);
	else if (ft_strchr(cmd[0], '/') == NULL && env[i])
	{
		while (env[i] && ft_strncmp(env[i], "PATH=", 5) != 0)
			i++;
		if (env[i])
			tmp_path = ft_split(&env[i][5], ':');
		if (!tmp_path)
			error_msg(ft_strdup("NULL"), cmd, env);
		path = get_path(cmd, tmp_path);
	}
	if (!path)
		error_msg(path, cmd, env);
	execve(path, cmd, env);
	error_msg(path, cmd, env);
}

t_word	*cmd_found(t_word *lexer)
{
	while (lexer)
	{
		if (lexer->token == CMD || lexer->token == DOUBLE_QUOTE
			|| lexer->token == SINGLE_QUOTE)
			return (lexer);
		if (lexer->token == PIPE)
			break ;
		lexer = lexer->next;
	}
	return (0);
}

void	child(int c_fd, int pipe_fd[2], int i, t_var *var)
{
	char	**env;
	char	**cmd;

	if (node_cmp_token(var->lexer, PARENTH_OPEN) == 0)
	{
		expand(var);
		if (var->error == false)
			do_wildcard(var);
		if (var->error == false)
			join_node(var);
	}
	(signal(SIGQUIT, SIG_DFL), do_dup(c_fd, pipe_fd, i, var));
	if (cmd_found(var->lexer) == 0)
		(close_all_fd(pipe_fd, c_fd, i, true), free_error(var, NULL, NULL, 0));
	do_cmd_in_parenth(c_fd, pipe_fd, i, var);
	(close_all_fd(pipe_fd, c_fd, i, true), var->in_fork = true);
	var->exit = true;
	if (is_builtins(cmd_found(var->lexer)) != 0)
	{
		g_exit_status = do_bultins(var);
		free_var(var);
		exit(g_exit_status);
	}
	env = split_env(var->env);
	if (!env)
		free_error(var, E_MALLOC, "env", 1);
	cmd = split_cmd(var);
	if (!cmd)
		free_error(var, E_MALLOC, "cmd", 1);
	free_var(var);
	if (cmd[0][0] == '\0')
		error_msg(NULL, cmd, env);
	exec(cmd, env);
}

int	fork_loop(t_var *var, int nb_cmd)
{
	int	i;
	int	pipe_fd[2];
	int	c_fd;

	i = 0;
	c_fd = -1;
	while (nb_cmd > i)
	{
		if (i != 0)
			c_fd = pipe_fd[0];
		if (pipe(pipe_fd) == -1)
			return (close_fd(c_fd, i), -1);
		if (choose_how_to_exec(c_fd, pipe_fd, i, var) == -1)
			return (-1);
		close_all_fd(pipe_fd, c_fd, i, false);
		i++;
		del_cmd(&var->lexer);
		if (var->execute_next == false)
			break ;
	}
	if (var->bonus_cmd == false)
		g_exit_status = wait_for_child(var->last_pid);
	return (close(pipe_fd[0]), unlink_here_doc(var), 0);
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
	if (nb_pipe == 0 && is_bonus_cmd(var->lexer) == 0
		&& is_builtins(cmd_found(var->lexer)) != 0)
	{
		g_exit_status = do_bultins(var);
		unlink_here_doc(var);
		if (var->exit == true)
			free_error(var, NULL, NULL, g_exit_status);
	}
	else if (fork_loop(var, nb_pipe + 1) == -1)
		free_error(NULL, E_PIPE, NULL, -99);
}
