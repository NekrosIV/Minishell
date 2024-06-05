/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:47:15 by pscala            #+#    #+#             */
/*   Updated: 2024/06/05 16:18:03 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_in_env(t_var *var)
{
	char	*old_pwd;
	char	*pwd;

	pwd = getcwd(NULL, 0);
	ft_putstr_fd("pwd =", 1);
	ft_putendl_fd(pwd, 1);
	if (!pwd)
		return (free_error(NULL, "pwd: " E_DIR_ERROR E_GETCWD_ERROR,
				strerror(errno), -1));
	old_pwd = find_in_env("$PWD", var);
	ft_putstr_fd("old pwd =", 1);
	ft_putendl_fd(old_pwd, 1);
	if (!old_pwd)
		old_pwd = ft_strdup("");
	replace_env(var, "$PWD", pwd);
	replace_env(var, "$OLDPWD", old_pwd);
	print_list_env(var->env);
	// old_pwd = find_in_env("$PWD", var);
	// ft_putstr_fd("old pwd =", 1);
	// ft_putendl_fd(old_pwd, 1);
}

int	cd_home(char *path, t_var *var, char *env)
{
	if (!path)
		return (free_error(NULL, "cd: ", env, -1), 1);
	else if (chdir(path) == -1)
		return (free(path), perror("chdir error"), 1);
	else
		replace_in_env(var);
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
		path = find_in_env("$OLDPWD", var);
		ft_putstr_fd("path =", 1);
		ft_putstr_fd(path, 1);
		ft_putstr_fd("\n", 1);
		return (cd_home(path, var, "$OLDPWD"));
	}
	else if (*cmd[1] && chdir(cmd[1]) == -1)
		return (perror("chdir error"), 1);
	else 
		replace_in_env(var);
		// print_list_env(var->env);
	return (0);
}
