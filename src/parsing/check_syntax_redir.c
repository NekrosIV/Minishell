/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_redir.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:13:12 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/28 17:38:03 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	check_token(t_word *tmp, t_word *start, t_var *var)
{
	if (tmp->token == DOL && start->token != HERE_DOC)
	{
		find_and_replace(tmp, var);
		if (tmp->token == DOL)
			return (free_error(var, E_REDIR, tmp->word, -1), -1);
	}
	if (tmp->token != CMD && tmp->token != QUOTE_CMD && tmp->token != DOL)
		return (free_error(var, E_SYNTAX, tmp->word, -2), -1);
	return (0);
}

t_word	*trim_rout(t_word *start, t_var *var)
{
	t_word	*tmp;

	tmp = skip_token(start->next->next, SPACES);
	if (check_token(tmp, start, var))
		return (NULL);
	trim_tword(&start, &tmp);
	return (tmp);
}

t_word	*check_and_trim(t_word *start, t_var *var)
{
	t_word	*tmp;

	tmp = start;
	if (start->token == REDIR_OUT && start->next->token == PIPE)
	{
		tmp = trim_rout(start, var);
		if (!tmp)
			return (NULL);
	}
	else
	{
		tmp = skip_token(start->next, SPACES);
		if (check_token(tmp, start, var))
			return (NULL);
		trim_tword(&start, &tmp);
	}
	start = tmp;
	if (start->prev == NULL)
		var->lexer = start;
	return (start);
}

int	check_syntax_redir(t_var *var)
{
	t_word	*start;

	start = var->lexer;
	while (start)
	{
		if (start->token == REDIR_IN || start->token == REDIR_OUT
			|| start->token == REDIR_APPEND || start->token == HERE_DOC)
		{
			start = check_and_trim(start, var);
			if (!start)
				return (-1);
		}
		start = start->next;
	}
	return (0);
}
