/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_git_branch.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:44:15 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/17 16:23:24 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	dup_for_git(char *filename)
{
	int	stderor;
	int	fd_out;

	fd_out = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out == -1)
		return (-1);
	stderor = open("/dev/null", O_RDONLY);
	if (stderor == -1)
		return (-1);
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		(perror("dup2(fd_out)"), close(fd_out));
		return (-1);
	}
	close(fd_out);
	if (dup2(stderor, STDERR_FILENO) == -1)
	{
		(perror("dup2(fd_in)"), close(stderor));
		return (-1);
	}
	close(stderor);
	return (0);
}

static char	*get_git_branch_two(int status, char *file_name)
{
	int		fd;
	char	*tmp;
	char	*line;

	if (status != 0)
		return (NULL);
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return (NULL);
	line = get_next_line(fd);
	close(fd);
	unlink(file_name);
	if (!line)
		return (NULL);
	tmp = ft_strtrim(line, "\n");
	if (!tmp)
		return (free(line), NULL);
	free(line);
	return (tmp);
}

char	*get_git_info(t_var *var, char *str, char *file_name)
{
	char	**cmd;
	char	**env;
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid == -1)
		return (NULL);
	if (pid == 0)
	{
		env = split_env(var->env);
		if (!env)
			free_error(var, E_MALLOC, "env", 1);
		cmd = ft_split(str, ' ');
		if (!cmd)
			(free_split(env), free_error(var, E_MALLOC, "cmd", 1));
		if (dup_for_git(file_name) == -1)
			(free_split(env), free_split(cmd), free_error(var, "dup err", NULL,
					1));
		exec(cmd, env);
	}
	waitpid(pid, &status, 0);
	return (get_git_branch_two(status, file_name));
}

char	*get_git_branch(t_var *var)
{
	char	*tmp;
	char	*tmp2;

	tmp = get_git_info(var, "git rev-parse --abbrev-ref HEAD",
			"/tmp/git_branch");
	if (!tmp)
		return (ft_strdup(""));
	tmp2 = ft_strjoin(RESET "] on [" BOLD CYAN, tmp);
	if (!tmp2)
		return (free(tmp), NULL);
	free(tmp);
	tmp = get_git_info(var, "git status --porcelain", "/tmp/git_status");
	if (!tmp)
		var->uncommitted_changes = false;
	else
		(var->uncommitted_changes = true, free(tmp));
	return (tmp2);
}
