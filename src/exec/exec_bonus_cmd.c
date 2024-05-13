/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 13:44:12 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/13 14:53:12 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_bonus_cmd(t_word *lexer)
{
	while (lexer->token != PIPE && lexer->token != END)
	{
		if (lexer->token == OR || lexer->token == AND)
			return (1);
		lexer = lexer->next;
	}
	return (0);
}

void	new_t_word(t_word **start, t_word **lexer)
{
	t_word	*end;
	t_word	*tmp;

	tmp = NULL;
	if ((*start)->prev != NULL)
		tmp = *lexer;
	*lexer = *start;
	end = *start;
	while (end->token != PIPE && end->token != END && end->token != OR
		&& end->token != AND)
		end = end->next;
	if ((*lexer)->prev != NULL)
		(*lexer)->prev->next = end->next;
	else
		tmp = end->next;
	(*lexer)->prev = NULL;
	end->next = NULL;
	if (end->token != PIPE)
		end->token = END;
	if (tmp != NULL)
		free_list_lexer(&tmp);
}

bool	update_execution_state(t_word **start, t_word *tmp, t_var *var,
		bool flag)
{
	if (tmp->token == AND || tmp->token == OR)
	{
		var->execute_next = (tmp->token == AND && g_exit_status == 0)
			|| (tmp->token == OR && g_exit_status != 0);
		*start = tmp->next;
		return (true);
	}
	return (flag);
}

t_word	*update_tmp(t_word *tmp, t_var *var)
{
	if (tmp->token == PARENTH_OPEN && var->execute_next == false)
		tmp = end_of_parenth(tmp);
	else
		tmp = tmp->next;
	return (tmp);
}

int	do_bonus_cmd(int c_fd, int pipe_fd[2], int i, t_var *var)
{
	pid_t	pid;
	t_word	*tmp;
	t_word	*start;
	bool	flag;

	tmp = var->lexer;
	start = tmp;
	flag = true;
	while (tmp != NULL)
	{
		if (var->execute_next && tmp->token == CMD && flag == true)
		{
			pid = fork();
			if (pid == -1)
				return (close_all_fd(pipe_fd, c_fd, i, true), -1);
			if (pid == 0)
				(new_t_word(&start, &var->lexer), child(c_fd, pipe_fd, i, var));
			g_exit_status = wait_for_child(pid);
			flag = false;
		}
		flag = update_execution_state(&start, tmp, var, flag);
		tmp = update_tmp(tmp, var);
	}
	return (0);
}
