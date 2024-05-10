/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 13:44:12 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/10 15:55:29 by kasingh          ###   ########.fr       */
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
void	new_t_word(t_word **start, t_var **var)
{
	t_word	*end;

	(*var)->tmp = (*var)->lexer;
	(*var)->lexer = *start;
	end = *start;
	while (end->token != PIPE && end->token != END && end->token != OR
		&& end->token != AND)
		end = end->next;
	if ((*var)->lexer->prev != NULL)
		(*var)->lexer->prev->next = end->next;
	(*var)->lexer->prev = NULL;
	end->next = NULL;
	end->token = END;
}
bool	update_execution_state(t_word **start, t_word *tmp, t_var *var,
		bool flag)
{
	if (tmp->token == AND || tmp->token == OR)
	{
		var->execute_next = (tmp->token == AND && exit_status == 0)
			|| (tmp->token == OR && exit_status != 0);
		*start = tmp->next;
		return (true);
	}
	return (flag);
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
				(new_t_word(&start, &var), child(c_fd, pipe_fd, i, var));
			(exit_status = wait_for_child(pid), flag = false);
		}
		flag = update_execution_state(&start, tmp, var, flag);
		if (tmp->token == PARENTH_OPEN && var->execute_next == false)
			tmp = end_of_parenth(tmp);
		tmp = tmp->next;
	}
	return (0);
}
