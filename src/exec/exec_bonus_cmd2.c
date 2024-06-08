/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus_cmd2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 20:29:19 by pscala            #+#    #+#             */
/*   Updated: 2024/06/08 20:33:55 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_bonus_cmd(t_word *lexer)
{
	while (lexer->token != PIPE && lexer->token != END)
	{
		if (lexer->token == OR || lexer->token == AND
			|| lexer->token == PARENTH_OPEN)
			return (1);
		lexer = lexer->next;
	}
	return (0);
}

void	new_lst(t_word **start, t_word **lexer, t_word *curent)
{
	t_word	*tmp;
	t_word	*end;

	tmp = NULL;
	end = NULL;
	if (curent->token == PARENTH_OPEN)
		end = end_of_parenth(curent);
	if ((*start)->prev != NULL)
		tmp = *lexer;
	*lexer = *start;
	if (end == NULL)
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
		if ((tmp->token == AND && g_exit_status == 0) || (tmp->token == OR
				&& g_exit_status != 0))
			var->execute_next = true;
		else
			var->execute_next = false;
		*start = tmp->next;
		return (true);
	}
	return (flag);
}

t_word	*update_tmp(t_word *tmp, t_var *var)
{
	(void)var;
	if (tmp->token == PARENTH_OPEN)
		tmp = end_of_parenth(tmp);
	else
		tmp = tmp->next;
	return (tmp);
}

t_word	*find_token(t_word *lexer, int token)
{
	while (lexer && lexer->token != token)
		lexer = lexer->next;
	return (lexer);
}
