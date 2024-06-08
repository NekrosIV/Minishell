/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_bonus_utils.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 20:39:31 by pscala            #+#    #+#             */
/*   Updated: 2024/06/08 20:40:13 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_before_and_affter(t_word *tmp, bool dir)
{
	if (dir == true)
	{
		tmp = tmp->next;
		while (tmp && (tmp->token == SPACES || tmp->token == END))
			tmp = tmp->next;
	}
	else
	{
		tmp = tmp->prev;
		while (tmp && (tmp->token == SPACES || tmp->token == END))
			tmp = tmp->prev;
	}
	if (!tmp || tmp->token == PIPE || tmp->token == OR || tmp->token == AND
		|| (tmp->token == PARENTH_CLOSE && dir == true)
		|| (tmp->token == PARENTH_OPEN && dir == false))
		return (-1);
	return (0);
}

int	check_syntax_or_and(t_word *lexer, t_var *var)
{
	t_word	*tmp;

	tmp = 0;
	while (lexer)
	{
		if (lexer->token == OR || lexer->token == AND)
		{
			tmp = lexer;
			if (check_before_and_affter(tmp, true) == -1
				|| check_before_and_affter(tmp, false) == -1)
				return (free_error(var, E_SYNTAX, lexer->word, -2), -1);
		}
		lexer = lexer->next;
	}
	return (0);
}

int	is_cmd(int token)
{
	if (token != SPACES && token != PARENTH_OPEN && token != PARENTH_CLOSE
		&& token != END && token != OR && token != AND && token != PIPE)
		return (1);
	return (0);
}

int	check_parenth_close(t_word *tmp, t_var *var)
{
	int	count;

	count = 0;
	while (tmp)
	{
		if (tmp->token == PARENTH_OPEN)
			count++;
		else if (tmp->token == PARENTH_CLOSE)
			count--;
		if (count < 0)
			break ;
		tmp = tmp->next;
	}
	if (count != 0)
	{
		free_error(var, E_EOF, ")", -2);
		return (free_error(var, E_S_EOF, NULL, -2), -1);
	}
	return (0);
}
