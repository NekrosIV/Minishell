/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 19:54:50 by pscala            #+#    #+#             */
/*   Updated: 2024/06/08 19:58:12 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_var	*init_var(t_env **envs)
{
	t_var	*var;

	var = malloc(sizeof(t_var));
	if (!var)
		(free_list_env(envs), free_error(NULL, E_MALLOC, "var", 1));
	var->exit = false;
	var->error = false;
	var->bonus_cmd = false;
	var->uncommitted_changes = false;
	var->in_fork = false;
	var->in_parenth = false;
	var->git = true;
	var->execute_next = true;
	var->line = NULL;
	var->lexer = NULL;
	var->envp = NULL;
	var->env = *envs;
	var->quoted_cmds = NULL;
	var->tmp = NULL;
	var->here_doc_count = 0;
	var->last_pid = 0;
	init_tab_builtins(&var);
	return (var);
}
