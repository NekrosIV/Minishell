/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:47:15 by pscala            #+#    #+#             */
/*   Updated: 2024/06/05 18:55:17 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	replace_in_env(t_var *var)
{
	char	*old_pwd;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (free_error(NULL, "cd: ..:", strerror(errno), -1), 1);
	old_pwd = find_in_env("$PWD", var);
	if (!old_pwd)
		old_pwd = ft_strdup("");
	replace_env(var, "$PWD", pwd);
	free(pwd);
	replace_env(var, "$OLDPWD", old_pwd);
	free(old_pwd);
	return (0);
}

int	ft_chdir(char *path, t_var *var, char *env)
{
	if (!path)
		return (free_error(NULL, "cd: ", env, -1), 1);
	else if (chdir(path) == -1)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (1);
	}
	else
		(replace_in_env(var));
	return (0);
}

int	cd(char **cmd, t_var *var)
{
	char	*path;

	if (!cmd[1])
	{
		path = find_in_env("$HOME", var);
		return (ft_chdir(path, var, "$HOME"), free(path), 0);
	}
	else if (cmd[2])
		return (free_error(NULL, "cd: ", "too many arguments", -1), 1);
	else if (ft_strcmp(cmd[1], "-") == 0)
	{
		path = find_in_env("$OLDPWD", var);
		if (ft_chdir(path, var, "$OLDPWD") == 0)
			return (ft_putendl_fd(path, 1), free(path), 0);
		return (free(path), 1);
	}
	else if (*cmd[1] && chdir(cmd[1]) == -1)
		return (ft_putstr_fd("cd: ", 2), perror(cmd[1]), 1);
	else if (replace_in_env(var) == 1)
		return (1);
	return (0);
}
