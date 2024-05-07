/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_pipe.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:11:28 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/07 13:10:26 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_pipe(t_word *tmp, bool dir)
{
	if (dir == true)
	{
		tmp = tmp->next;
		while (tmp)
		{
			if (tmp->token == SPACES || tmp->token == END)
				tmp = tmp->next;
			else
				break ;
		}
	}
	else
	{
		tmp = tmp->prev;
		while (tmp)
		{
			if (tmp->token == SPACES || tmp->token == END)
				tmp = tmp->prev;
			else
				break ;
		}
	}
	if (!tmp || tmp->token == PIPE)
		return (-1);
	return (0);
}

int	check_syntax_pipe(t_word *lexer, t_var *var)
{
	t_word	*tmp;

	tmp = 0;
	while (lexer)
	{
		if (lexer->token == PIPE)
		{
			tmp = lexer;
			if (check_pipe(tmp, true) == -1 || check_pipe(tmp, false) == -1)
				return (free_error(var, E_SYNTAX, lexer->word, -2), -1);
		}
		lexer = lexer->next;
	}
	return (0);
}
