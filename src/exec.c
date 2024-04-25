/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 15:26:21 by pscala            #+#    #+#             */
/*   Updated: 2024/04/25 17:13:55 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	**get_cmd(char *cmd)
{
	char	**cmd_split;

	cmd_split = ft_split(cmd, ' ');
	if (!cmd_split || !cmd_split[0])
	{
		ft_putstr_fd("Command not found: ", 2);
		ft_putendl_fd(cmd, 2);
		free(cmd_split);
		exit(127);
	}
	return (cmd_split);
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
