/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 16:44:47 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/09 21:14:16 by kasingh          ###   ########.fr       */
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
			error_msg(path, cmd, env);
		path = get_path(cmd, tmp_path);
	}
	if (!path)
		error_msg(path, cmd, env);
	execve(path, cmd, env);
	error_msg(path, cmd, env);
}
void	print_cmd(char **cmd)
{
	int	i;

	i = 0;
	while (cmd[i])
	{
		printf("cmd[%d]: %s\n", i, cmd[i]);
		i++;
	}
}
void	prepare_to_exe(t_var *var)
{
	char	**env;
	char	**cmd;

	env = split_env(var->env);
	if (!env)
		free_error(var, E_MALLOC, "env", 1);
	cmd = split_cmd(var);
	if (!cmd)
		free_error(var, E_MALLOC, "cmd", 1);
	var->exit = true;
	free_var(var);
	if (cmd[0][0] == '\0')
	{
		ft_putstr_fd(cmd[0], 2);
		ft_putendl_fd(": command not found", 2);
		free_split(env);
		free_split(cmd);
		exit(127);
	}
	exec(cmd, env);
}
int	is_bonus_cmd(t_word *lexer)
{
	while (lexer->token != PIPE && lexer->token != END)
	{
		if (lexer->token == OR || lexer->token == AND)
			return (1);
		lexer = lexer->next;
	}
	return (0);
}
void	new_t_word(t_word **start, t_word **lexer)
{
	t_word	*end;
	t_word	*tmp;
	t_word	*del;

	tmp = *lexer;
	*lexer = *start;
	end = *start;
	while (end->token != PIPE && end->token != END && end->token != OR
		&& end->token != AND)
		end = end->next;
	end->next = NULL;
	end->token = END;
	(*lexer)->prev = NULL;
}

int	do_bonus_cmd(int c_fd, int pipe_fd[2], int i, t_var *var)
{
	pid_t	pid;
	t_word	*tmp;
	t_word	*start;
	bool	flag;

	tmp = var->lexer;
	start = tmp;
	flag = true;
	while (tmp != NULL)
	{
		if (var->execute_next && tmp->token == CMD && flag == true)
		{
			pid = fork();
			if (pid == -1)
			{
				handle_fork_error(pid, pipe_fd, c_fd, i);
				return (-1);
			}
			if (pid == 0)
			{
				new_t_word(&start, &var->lexer);
				child(c_fd, pipe_fd, i, var);
			}
			waitpid(pid, &exit_status, 0);
			flag = false;
		}
		if (tmp->token == AND || tmp->token == OR)
		{
			var->execute_next = (tmp->token == AND && exit_status == 0)
				|| (tmp->token == OR && exit_status != 0);
			(start = tmp->next, flag = true);
		}
		if (tmp->token == PARENTH_OPEN && var->execute_next == false)
			tmp = end_of_parenth(tmp);
		tmp = tmp->next;
	}
	return (0);
}

void	child(int c_fd, int pipe_fd[2], int i, t_var *var)
{
	char	**env;
	char	**cmd;

	signal(SIGQUIT, SIG_DFL);
	do_dup(c_fd, pipe_fd, i, var);
	if (node_cmp_token(var->lexer, CMD) == 0 && node_cmp_token(var->lexer,
			SINGLE_QUOTE) == 0 && node_cmp_token(var->lexer, DOUBLE_QUOTE) == 0)
		free_error(var, NULL, NULL, 0);
	prepare_to_exe(var);
}

void	handle_fork_error(pid_t pid, int pipe_fd[2], int c_fd, int i)
{
	close(pipe_fd[0]);
	close_fd(c_fd, i);
	close(pipe_fd[1]);
}
int	do_simple_cmd(int c_fd, int pipe_fd[2], int i, t_var *var)
{
	var->last_pid = fork();
	if (var->last_pid == -1)
		return (handle_fork_error(var->last_pid, pipe_fd, c_fd, i), -1);
	if (var->last_pid == 0)
		child(c_fd, pipe_fd, i, var);
	var->bonus_cmd = false;
	return (0);
}
int	fork_loop(t_var *var, int nb_cmd)
{
	int		i;
	int		pipe_fd[2];
	int		c_fd;
	pid_t	pid;

	(i = 0, c_fd = 0);
	while (nb_cmd > i)
	{
		if (i != 0)
			c_fd = pipe_fd[0];
		if (pipe(pipe_fd) == -1)
			return (close_fd(c_fd, i), -1);
		if (is_bonus_cmd(var->lexer) == 1)
		{
			if (do_bonus_cmd(c_fd, pipe_fd, i, var) == -1)
				return (-1);
		}
		else if (do_simple_cmd(c_fd, pipe_fd, i, var) == -1)
			return (-1);
		(close(pipe_fd[1]), close_fd(c_fd, i));
		i++;
		if (var->execute_next == false)
			break ;
		del_cmd(&var->lexer);
	}
	if (var->bonus_cmd == false)
		exit_status = wait_for_child(var->last_pid);
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
	if (fork_loop(var, nb_pipe + 1) == -1)
		free_error(NULL, E_PIPE, NULL, -99);
}
