/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 16:46:34 by pscala            #+#    #+#             */
/*   Updated: 2024/04/18 18:15:50 by pscala           ###   ########.fr       */
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

int	check_syntax_pipe(t_word *lexer)
{
	t_word	*tmp;

	while (lexer)
	{
		if (lexer->token == PIPE)
		{
			tmp = lexer;
			if (check_pipe(tmp, true) == -1 || check_pipe(tmp, false) == -1)
				return (free_error(NULL, E_syntax, lexer->word, -99), -1);
		}
		lexer = lexer->next;
	}
	return (0);
}

void	check_syntax(t_var *var)
{
	if (check_syntax_pipe(var->lexer) == -1)
		var->error = true;
}