/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:15:08 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/18 16:49:38 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <unistd.h>

char	*get_pwd(t_var *var)
{
	char	*pwd;
	char	*home;
	char	*tmp;

	pwd = find_in_env("$PWD", var);
	if (!pwd)
		pwd = ft_strdup("");
	home = find_in_env("$HOME", var);
	if (home && ft_strncmp(pwd, home, ft_strlen(home)) == 0)
	{
		tmp = ft_strjoin("~", pwd + ft_strlen(home));
		if (!tmp)
			free_error(var, E_MALLOC, "tmp", 1);
		(free(pwd), free(home));
		pwd = tmp;
	}
	return (pwd);
}

char	*get_prompt_two(t_var *var, char *tmp2, char *git_branch)
{
	char	*prompt;
	char	*tmp;

	if (var->uncommitted_changes == true)
		tmp = ft_strjoin(tmp2, RESET "]" RED "✘" RESET "\n╰──");
	else if (var->uncommitted_changes == false && git_branch[0] != '\0')
		tmp = ft_strjoin(tmp2, RESET "]" BOLD CYAN "✔" RESET "\n╰──");
	else
		tmp = ft_strjoin(tmp2, RESET "]\n╰──");
	free(git_branch);
	if (!tmp)
		(free(tmp2), free_error(var, E_MALLOC, "tmp", 1));
	if (g_exit_status == 0)
		prompt = ft_strjoin(tmp, RESET "➤ ");
	else
		prompt = ft_strjoin(tmp, RED "➤ " RESET);
	if (!prompt)
		(free(tmp), free(tmp2), free_error(var, E_MALLOC, "prompt", 1));
	(free(tmp), free(tmp2));
	return (prompt);
}

char	*get_prompt(t_var *var)
{
	char	*prompt;
	char	*pwd;
	char	*tmp2;
	char	*git_branch;

	tmp2 = NULL;
	git_branch = get_git_branch(var);
	if (!git_branch)
		free_error(var, E_MALLOC, "git_branch", 1);
	pwd = get_pwd(var);
	if (!pwd)
		(free(git_branch), free_error(var, E_MALLOC, "pwd", 1));
	prompt = ft_strjoin("╭─[" BOLD PURPLE "minirt" RESET "] in [" BOLD PURPLE,
			pwd);
	free(pwd);
	if (!prompt)
		(free(git_branch), free_error(var, E_MALLOC, "prompt", 1));
	tmp2 = ft_strjoin(prompt, git_branch);
	if (!tmp2)
		(free(git_branch), free(tmp2), free_error(var, E_MALLOC, "tmp2", 1));
	free(prompt);
	return (get_prompt_two(var, tmp2, git_branch));
}

void	get_line(t_var *var)
{
	char	*str;
	char	*prompt;

	str = NULL;
	prompt = get_prompt(var);
	if (isatty(0))
		var->line = readline(prompt);
	else
	{
		str = get_next_line(0);
		if (!str)
			var->line = NULL;
		else
			var->line = ft_strtrim(str, "\n");
	}
	free(str);
	free(prompt);
}

t_var	*init_var(t_env **envs, bool allias_ls)
{
	t_var	*var;

	var = malloc(sizeof(t_var));
	if (!var)
	{
		free_list_env(envs);
		free_error(NULL, E_MALLOC, "var", 1);
	}
	var->exit = false;
	var->error = false;
	var->bonus_cmd = false;
	var->uncommitted_changes = false;
	var->use_ls_alias = allias_ls;
	var->execute_next = true;
	var->line = NULL;
	var->lexer = NULL;
	var->envp = NULL;
	var->env = *envs;
	var->quoted_cmds = NULL;
	var->tmp = NULL;
	var->here_doc_count = 0;
	var->last_pid = 0;
	return (var);
}
