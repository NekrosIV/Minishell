/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_git_branch.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 13:44:15 by kasingh           #+#    #+#             */
/*   Updated: 2024/06/08 13:38:43 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	dup_for_git(char *filename)
{
	int	stderor;
	int	fd_out;

	stderor = open("/dev/null", O_RDONLY);
	if (stderor == -1)
		return (-1);
	if (dup2(stderor, STDERR_FILENO) == -1)
	{
		(perror("dup2(fd_in)"), close(stderor));
		return (-1);
	}
	close(stderor);
	fd_out = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd_out == -1)
		return (perror(filename), -1);
	if (dup2(fd_out, STDOUT_FILENO) == -1)
	{
		(perror("dup2(fd_out)"), close(fd_out));
		return (-1);
	}
	close(fd_out);
	return (0);
}

static char	*get_git_branch_two(int status, char *file_name)
{
	int		fd;
	char	*tmp;
	char	*line;
	ssize_t	bytes_read;
	char	buffer[20];

	if (status != 0)
		return (NULL);
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		return (NULL);
	if (ft_strncmp(file_name, "git_status", 10) == 0)
	{
		bytes_read = read(fd, buffer, 10);
		if (bytes_read == -1 || bytes_read == 0)
			return (unlink(file_name), close(fd), NULL);
		return (unlink(file_name), close(fd), "yes");
	}
	line = get_next_line(fd);
	if (!line)
		return (unlink(file_name), close(fd), NULL);
	tmp = ft_strtrim(line, "\n");
	if (!tmp)
		return (unlink(file_name), close(fd), free(line), NULL);
	return (unlink(file_name), close(fd), free(line), tmp);
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
			(free_split(env), free_split(cmd), free_error(var, NULL, NULL, 1));
		exec(cmd, env);
	}
	waitpid(pid, &status, 0);
	return (get_git_branch_two(status, file_name));
}

char	*get_git_branch(t_var *var)
{
	char	*tmp;
	char	*tmp2;
	char	*tmp3;

	if (var->git == false)
		return (ft_strdup(""));
	tmp = NULL;
	tmp = get_git_info(var, "/usr/bin/git rev-parse --abbrev-ref HEAD",
			"git_branch");
	if (!tmp)
		return (ft_strdup(""));
	tmp3 = get_git_info(var, "/usr/bin/git status --porcelain", "git_status");
	if (!tmp3)
		var->uncommitted_changes = false;
	else
		var->uncommitted_changes = true;
	tmp2 = ft_strjoin(RESET BOLD TEAL " git:(", tmp);
	if (!tmp2)
		return (free(tmp), NULL);
	free(tmp);
	return (tmp2);
}
