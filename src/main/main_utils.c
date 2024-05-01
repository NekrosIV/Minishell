/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:15:08 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/01 12:19:15 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_var	*init_var(t_env **envs, int exit_statut)
{
	t_var	*var;

	var = malloc(sizeof(t_var));
	if (!var)
	{
		free_list_env(envs);
		free_error(NULL, E_MALLOC, "var", 1);
	}
	if (isatty(0))
		var->prompt = PROMPT;
	else
		var->prompt = "";
	var->exit = false;
	var->error = false;
	var->line = NULL;
	var->lexer = NULL;
	var->envp = NULL;
	var->env = *envs;
	var->status = exit_statut;
	var->quoted_cmds = NULL;
	return (var);
}
