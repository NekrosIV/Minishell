/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:28:47 by kasingh           #+#    #+#             */
/*   Updated: 2024/06/01 15:01:40 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	if (ft_strcmp("cd", tmp->word) == 0)
		return (1);
	else if (ft_strcmp("pwd", tmp->word) == 0)
		return (1);
	else if (ft_strcmp("export", tmp->word) == 0)
		return (1);
	else if (ft_strcmp("unset", tmp->word) == 0)
		return (1);
	else if (ft_strcmp("echo", tmp->word) == 0)
		return (1);
	else if (ft_strcmp("env", tmp->word) == 0)
		return (1);
	else if (ft_strcmp("exit", tmp->word) == 0)
		return (1);
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
	if (var->in_fork == false)
	{
		old_fd_in = dup(STDIN_FILENO);
		if (old_fd_in == -1)
			free_error(var, "dup()", strerror(errno), 1);
		old_fd_out = dup(STDOUT_FILENO);
		if (old_fd_out == -1)
			free_error(var, "dup()", strerror(errno), 1);
		while (tmp->token != END)
		{
			if (tmp->token == REDIR_IN || tmp->token == HERE_DOC)
			{
			}
		}
	}
	// status_exit = exe_builtins(cmd, var);
	return (status_exit);
}
