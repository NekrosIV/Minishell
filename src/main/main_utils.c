/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:15:08 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/16 19:07:27 by kasingh          ###   ########.fr       */
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
		free(pwd), free(home);
		pwd = tmp;
	}
	return (pwd);
}

void	handle_stderror(bool do_it)
{
	int			fd;
	static int	old_fd = -1;

	if (do_it == true)
	{
		old_fd = dup(STDERR_FILENO);
		fd = open("/dev/null", O_RDONLY);
		if (fd == -1)
			return ;
		if (dup2(fd, STDERR_FILENO) == -1)
		{
			perror("dup2(fd_in)"), close(fd);
			return ;
		}
		close(fd);
	}
	else if (old_fd != -1)
	{
		if (dup2(old_fd, STDERR_FILENO) == -1)
		{
			perror("dup2(old_fd)");
			return ;
		}
		close(old_fd);
	}
}

char	*get_git_branch(t_var *var)
{
	int		fd;
	char	*branch;
	char	*tmp;

	handle_stderror(true);
	var->line = ft_strdup("git rev-parse --abbrev-ref HEAD > git_brach_name");
	if (!var->line)
		free_error(var, E_MALLOC, "var->line", 1);
	parsing(var);
	if (var->error == true)
		return (free_list_lexer(&var->lexer), free(var->line), NULL);
	before_exe(var);
	if (var->error == true)
		return (free_list_lexer(&var->lexer), free(var->line), NULL);
	fd = open("git_brach_name", O_RDONLY);
	if (fd == -1)
		return (free_list_lexer(&var->lexer), free(var->line), NULL);
	tmp = get_next_line(fd), close(fd), unlink("git_brach_name");
	if (!tmp)
		return (free_list_lexer(&var->lexer), free(var->line), NULL);
	branch = ft_strtrim(tmp, "\n");
	if (!branch)
		return (free_list_lexer(&var->lexer), free(var->line), NULL);
	handle_stderror(false), free(tmp);
	return (free_list_lexer(&var->lexer), free(var->line), branch);
}
char	*get_prompt(t_var *var)
{
	char	*prompt;
	char	*tmp;
	char	*pwd;
	char	*tmp2;
	char	*git_branch;

	tmp2 = NULL;
	tmp2 = get_git_branch(var);
	if (!tmp2)
	{
		git_branch = ft_strdup("");
		g_exit_status = 0;
	}
	else
		git_branch = ft_strjoin(RESET "] in [" BOLD CYAN, tmp2);
	free(tmp2);
	pwd = get_pwd(var);
	prompt = ft_strjoin("╭─[" GREEN "minirt" RESET "] in [" BLUE, pwd);
	if (!prompt)
		free_error(var, E_MALLOC, "prompt", 1);
	tmp2 = ft_strjoin(prompt, git_branch);
	tmp = ft_strjoin(tmp2, RESET "]\n╰──");
	if (!tmp)
		free_error(var, E_MALLOC, "tmp", 1);
	free(prompt), free(tmp2), free(git_branch);
	if (g_exit_status == 0)
		prompt = ft_strjoin(tmp, RESET "➤ ");
	else
		prompt = ft_strjoin(tmp, RED "➤ " RESET);
	free(tmp), free(pwd);
	return (prompt);
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
