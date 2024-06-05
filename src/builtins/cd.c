/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:47:15 by pscala            #+#    #+#             */
/*   Updated: 2024/06/05 17:13:27 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_in_env(t_var *var)
{
	char	*old_pwd;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (free_error(NULL, "pwd: " E_DIR_ERROR E_GETCWD_ERROR,
				strerror(errno), -1));
	old_pwd = find_in_env("$PWD", var);
	if (!old_pwd)
		old_pwd = ft_strdup("");
	replace_env(var, "$PWD", pwd);
	free(pwd);
	replace_env(var, "$OLDPWD", old_pwd);
	free(old_pwd);
}

int	ft_chdir(char *path, t_var *var, char *env)
{
	if (!path)
		return (free_error(NULL, "cd: ", env, -1), 1);
	else if (chdir(path) == -1)
		return (free(path), perror("chdir error"), 1);
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
		return (ft_chdir(path, var, "$OLDPWD"), ft_putendl_fd(path, 1), free(path), 0);
	}
	else if (*cmd[1] && chdir(cmd[1]) == -1)
		return (perror("chdir error"), 1);
	else
		replace_in_env(var);
	return (0);
}
