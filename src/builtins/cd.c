/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:47:15 by pscala            #+#    #+#             */
/*   Updated: 2024/06/04 16:35:20 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_in_env(char *env, t_var *var)
{
	char	*old_pwd;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	if (!pwd)
		return (free_error(NULL, "pwd: " E_DIR_ERROR E_GETCWD_ERROR,
				strerror(errno), -1));
	old_pwd = find_in_env(env, var);
	if (!old_pwd)
		old_pwd = ft_strdup("");
	replace_env(var, "$PWD", pwd);
	replace_env(var, "$OLDPWD", old_pwd);
}

int	cd_home(char *path, t_var *var, char *env)
{
	if (!path)
		return (free_error(NULL, "cd: ", env, -1), 1);
	else if (chdir(path) == -1)
		return (free(path), perror("chdir error"), 1);
	else
		replace_in_env(env, var);
	free(path);
	return (0);
}

int	cd(char **cmd, t_var *var)
{
	char	*path;

	if (!cmd[1])
	{
		path = find_in_env("$HOME", var);
		return (cd_home(path, var, "$HOME"));
	}
	else if (cmd[2])
		return (free_error(NULL, "cd: ", "too many arguments", -1), 1);
	else if (ft_strcmp(cmd[1], "-") == 0)
	{
		print_list_env(var->env);
		path = find_in_env("$OLDPWD", var);
		ft_putstr_fd("path =", 1);
		ft_putstr_fd(path, 1);
		ft_putstr_fd("\n", 1);
		return (cd_home(path, var, "$OLDPWD"));
	}
	else if (*cmd[1])
	{
		path = find_in_env(cmd[1], var);
		return (cd_home(path, var, "$HOME"));
	}
	return (0);
}
