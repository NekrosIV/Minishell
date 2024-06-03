/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:28:47 by kasingh           #+#    #+#             */
/*   Updated: 2024/06/03 12:50:37 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	replace_env(t_var *var, char *str, char *new_env_value)
{
	t_env	*envp;
	int		len;
	char	*tmp;
	
	envp = var->env;
	len = ft_strlen(str);
	while (envp)
	{
		if (ft_strncmp(&str[1], envp->line, len - 1) == 0 && envp->line[len
				- 1] == '=')
		{
			str = ft_strjoin(str, "=");
			if (!str)
    			return (free_error(var, E_MALLOC, "replace_env", 1), -1);
			tmp = ft_strjoin(str, new_env_value);
			if (!tmp)
    			return (free_error(var, E_MALLOC, "replace_env", 1), -1);
			free(envp->line);
			envp->line = tmp;
			free(str);
			return (0);
		}
		envp = envp->next;
	}
	return (-1);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if ((unsigned char)(s1[i]) > (unsigned char)(s2[i]))
			return (1);
		if ((unsigned char)(s1[i]) < (unsigned char)(s2[i]))
			return (-1);
		i++;
	}
	return (0);
}

int	is_builtins(t_word *tmp)
{
	if (tmp == 0)
		return (0);
	if (ft_strcmp("cd", tmp->word) == 0)
		return (1);
	else if (ft_strcmp("pwd", tmp->word) == 0)
		return (2);
	else if (ft_strcmp("export", tmp->word) == 0)
		return (3);
	else if (ft_strcmp("unset", tmp->word) == 0)
		return (4);
	else if (ft_strcmp("echo", tmp->word) == 0)
		return (5);
	else if (ft_strcmp("env", tmp->word) == 0)
		return (6);
	else if (ft_strcmp("exit", tmp->word) == 0)
		return (7);
	return (0);
}

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
			do_dup_in_builtins(tmp);
		else if (tmp->token == REDIR_OUT || tmp->token == REDIR_APPEND)
			do_dup_out_builtins(tmp);
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

int	do_bultins(t_var *var)
{
	char	**cmd;
	int		old_fd_in;
	int		old_fd_out;
	int		status_exit;
	t_word	*tmp;

	status_exit = 0;
	old_fd_in = -1;
	old_fd_out = -1;
	tmp = var->lexer;
	cmd = split_cmd(var);
	if (!cmd)
		free_error(var, E_MALLOC, "cmd", 1);
	if (var->in_fork == false && is_dir_in_cmd(tmp) == 1)
	{
		if (do_dup_builtins(tmp, &old_fd_in, &old_fd_out) == 1)
			return (free_split(cmd), EXIT_FAILURE);
	}
	status_exit = var->tab_builtins[is_builtins(cmd_found(var->lexer))](cmd,
			var);
	if (restore_fds(old_fd_in, old_fd_out))
		return (free_split(cmd), EXIT_FAILURE);
	return (free_split(cmd), status_exit);
}
