/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:10:54 by kasingh           #+#    #+#             */
/*   Updated: 2024/06/08 20:40:25 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_word	*end_of_parenth(t_word *lexer)
{
	int		count;
	t_word	*tmp;

	if (lexer->token == PARENTH_CLOSE)
		return (lexer);
	count = 1;
	tmp = lexer->next;
	while (tmp)
	{
		if (tmp->token == PARENTH_OPEN)
			count++;
		if (tmp->token == PARENTH_CLOSE)
			count--;
		if (count == 0)
			break ;
		tmp = tmp->next;
	}
	return (tmp);
}

int	check_out_pareth(t_word *lexer)
{
	t_word	*tmp;

	tmp = skip_token(lexer->prev, SPACES, false);
	if (tmp != NULL && (tmp->token == CMD || tmp->token == DOL
			|| tmp->token == SINGLE_QUOTE || tmp->token == DOUBLE_QUOTE
			|| tmp->token == PARENTH_CLOSE))
		return (-1);
	tmp = end_of_parenth(lexer);
	tmp = skip_token(tmp->next, SPACES, true);
	if (tmp != NULL && (tmp->token == CMD || tmp->token == DOL
			|| tmp->token == SINGLE_QUOTE || tmp->token == DOUBLE_QUOTE
			|| tmp->token == PARENTH_OPEN))
		return (-1);
	return (0);
}

int	check_in_parenth2(t_word *tmp, bool cmd, bool parenthsis)
{
	tmp = tmp->prev;
	while (tmp->token == SPACES || is_cmd(tmp->token))
		tmp = tmp->prev;
	if (cmd == true && tmp->token == PARENTH_OPEN)
		return (0);
	else if (tmp->token == PARENTH_CLOSE && parenthsis == true)
		return (-1);
	else if (tmp != NULL && tmp->token != OR && tmp->token != AND
		&& tmp->token != PARENTH_CLOSE)
		return (-1);
	return (0);
}

int	check_in_parenth(t_word *lexer)
{
	t_word	*tmp;
	bool	parenthesis;
	bool	cmd;

	cmd = false;
	tmp = lexer->next;
	parenthesis = false;
	while (tmp->token == SPACES || is_cmd(tmp->token))
	{
		if (is_cmd(tmp->token))
			cmd = true;
		tmp = tmp->next;
	}
	if (cmd == true && tmp->token == PARENTH_CLOSE)
		cmd = true;
	else if (tmp->token == PARENTH_OPEN)
		parenthesis = true;
	else if (tmp != NULL && tmp->token != OR && tmp->token != AND
		&& tmp->token != PARENTH_OPEN)
		return (-1);
	return (check_in_parenth2(end_of_parenth(tmp), cmd, parenthesis));
}

int	check_syntax_parenth(t_word *lexer, t_var *var)
{
	if (check_parenth_close(lexer, var) == -1)
		return (-1);
	while (lexer)
	{
		if (lexer->token == PARENTH_OPEN && (check_out_pareth(lexer) == -1
				|| check_in_parenth(lexer) == -1))
			return (free_error(var, E_SYNTAX, "(", -2), -1);
		lexer = lexer->next;
	}
	return (1);
}
