/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 16:42:40 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/27 17:17:56 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	del_cmd(t_word **word)
{
	t_word	*tmp;

	while ((*word)->token != PIPE && (*word)->token != END)
	{
		tmp = *word;
		(*word) = (*word)->next;
		del_tword(&tmp);
	}
	if ((*word)->token == PIPE)
	{
		tmp = *word;
		(*word) = (*word)->next;
		del_tword(&tmp);
	}
}

void	error_msg(char *path, char **cmd, char **env)
{
	if (!path && ft_strchr(cmd[0], '/') != NULL)
		ft_putstr_fd("No such file or directory : ", 2);
	else if (!path)
		ft_putstr_fd("command not found: ", 2);
	else if (access(path, F_OK) == 0 && access(path, X_OK) != 0)
	{
		ft_putstr_fd("Permission denied: ", 2);
		ft_putendl_fd(cmd[0], 2);
		free_split(cmd);
		free_split(env);
		free(path);
		exit(126);
	}
	if (path)
		free(path);
	ft_putendl_fd(cmd[0], 2);
	free_split(cmd);
	free_split(env);
	exit(127);
}

void	close_fd(int fd, int i)
{
	if (i != 0)
		close(fd);
}

char	*get_path(char **cmd, char **path)
{
	int		i;
	char	*good_path;
	char	*tmp;
	char	*slash_cmd;

	good_path = NULL;
	slash_cmd = ft_strjoin("/", cmd[0]);
	if (!slash_cmd)
		return (NULL);
	i = 0;
	while (path[i])
	{
		tmp = ft_strjoin(path[i], slash_cmd);
		if (!tmp)
			return (free(slash_cmd), NULL);
		if (access(tmp, F_OK) == 0)
		{
			good_path = ft_strdup(tmp);
			if (!good_path)
				return (free(tmp), NULL);
		}
		free(tmp);
		i++;
	}
	return (free_split(path), free(slash_cmd), good_path);
}

int	wait_for_child(pid_t pid)
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
