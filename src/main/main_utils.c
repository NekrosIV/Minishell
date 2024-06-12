/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:15:08 by kasingh           #+#    #+#             */
/*   Updated: 2024/06/12 11:10:39 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_pwd(t_var *var)
{
	char	*pwd;
	char	*tmp;
	int		i;

	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		var->git = false;
		pwd = ft_strdup("???");
		if (!pwd)
			return (NULL);
		return (pwd);
	}
	i = ft_strlen(pwd);
	while (i > 0)
	{
		if (pwd[i] == '/')
			break ;
		i--;
	}
	tmp = ft_strdup(&pwd[++i]);
	free(pwd);
	pwd = tmp;
	return (pwd);
}

char	*get_prompt_two(t_var *var, char *tmp2, char *git_branch)
{
	char	*tmp;

	if (var->uncommitted_changes == true)
		tmp = ft_strjoin(tmp2, ")" RESET RED "✘ " RESET);
	else if (var->uncommitted_changes == false && git_branch[0] != '\0')
		tmp = ft_strjoin(tmp2, ")" RESET LIGHT_GREEN "✔ " RESET);
	else
		tmp = ft_strjoin(tmp2, " " RESET);
	free(git_branch);
	if (!tmp)
		(free(tmp2), free_error(var, E_MALLOC, "tmp", 1));
	free(tmp2);
	return (tmp);
}

char	*get_prompt(t_var *var)
{
	char	*prompt;
	char	*pwd;
	char	*tmp2;
	char	*git_branch;

	tmp2 = NULL;
	pwd = get_pwd(var);
	if (!pwd)
		free_error(var, E_MALLOC, "pwd", 1);
	git_branch = get_git_branch(var);
	if (!git_branch)
		(free(pwd), free_error(var, E_MALLOC, "git_branch", 1));
	if (g_exit_status == 0)
		prompt = ft_strjoin(GREEN "➜ " BOLD LILA "minirt " BOLD O_BLUE, pwd);
	else
		prompt = ft_strjoin(RED "➜ " BOLD LILA "minirt " BOLD O_BLUE, pwd);
	free(pwd);
	if (!prompt)
		(free(git_branch), free_error(var, E_MALLOC, "prompt", 1));
	tmp2 = ft_strjoin(prompt, git_branch);
	if (!tmp2)
		(free(git_branch), free(tmp2), free_error(var, E_MALLOC, "tmp2", 1));
	return (free(prompt), get_prompt_two(var, tmp2, git_branch));
}

void	get_line(t_var *var)
{
	char	*str;
	char	*prompt;

	str = NULL;
	if (isatty(0))
	{
		prompt = get_prompt(var);
		var->line = readline(prompt);
		free(prompt);
	}
	else
	{
		str = get_next_line(0);
		if (!str)
			var->line = NULL;
		else
			var->line = ft_strtrim(str, "\n");
	}
	free(str);
}

void	init_tab_builtins(t_var **var)
{
	(*var)->tab_builtins[0] = 0;
	(*var)->tab_builtins[1] = cd;
	(*var)->tab_builtins[2] = pwd;
	(*var)->tab_builtins[3] = export;
	(*var)->tab_builtins[4] = unset;
	(*var)->tab_builtins[5] = echo;
	(*var)->tab_builtins[6] = env;
	(*var)->tab_builtins[7] = exit_builtin;
}
