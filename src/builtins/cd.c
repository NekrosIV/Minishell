/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:47:15 by pscala            #+#    #+#             */
/*   Updated: 2024/06/03 18:21:29 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_in_env(t_var *var, char *env)
{
	char	*old_pwd;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (free_error(NULL, "pwd: " E_DIR_ERROR E_GETCWD_ERROR,
				strerror(errno), -1));
	old_pwd = find_in_env(env, var);
}

int	cd_home(char *path, t_var *var)
{
	if (!path)
		return (free_error(NULL, "cd: ", "\"HOME\" is not defined", -1), 1);
	else if (chdir(path) == -1)
		return (free(path), perror("chdir error"), 1);
	else
		replace_in_env(var, "$PWD");
	free(path);
}

int	cd(char **cmd, t_var *var)
{
	char	*path;

	if (var->in_fork == true)
		return (0);
	if (!cmd[1])
	{
		path = find_in_env("$HOME", var);
		return (cd_home(path, var));
	}
	else if (cmd[2])
	{
		free_error(NULL, "cd: ", "too many arguments", -1);
		return (1);
	}
	return (0);
}
