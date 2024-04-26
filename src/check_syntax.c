/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 16:46:34 by pscala            #+#    #+#             */
/*   Updated: 2024/04/26 16:06:40 by kasingh          ###   ########.fr       */
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
				return (free_error(var, E_syntax, lexer->word, -2), -1);
		}
		lexer = lexer->next;
	}
	return (0);
}

void	del_tword(t_word **word)
{
	t_word	*temp;
	t_word	*prev;

	temp = *word;
	*word = (*word)->next;
	if (temp->prev == NULL)
		prev = NULL;
	else
	{
		prev = temp->prev;
		prev->next = (*word);
	}
	(*word)->prev = prev;
	if (temp->token == HERE_DOC && !ft_strncmp(temp->word, "<<", 2))
		unlink(temp->word);
	free(temp->word);
	free(temp);
}

void	trim_tword(t_word **start, t_word **end)
{
	t_word	*temp;
	int		token;

	token = (*start)->token;
	while ((*start)->token != (*end)->token && (*start)->token != END)
	{
		temp = *start;
		*start = (*start)->next;
		del_tword(&temp);
	}
	(*end)->token = token;
}

t_word	*skip_space(t_word *tmp)
{
	while (tmp)
	{
		if (tmp->token == SPACES)
			tmp = tmp->next;
		else
			break ;
	}
	return (tmp);
}

bool	check_token(t_word *tmp, t_word *start, t_var *var)
{
	if (tmp->token == DOL && start->token != HERE_DOC)
	{
		find_and_replace(tmp, var);
		if (tmp->token == DOL)
			return (free_error(var, E_REDIR, tmp->word, -1), -1);
	}
	if (tmp->token != CMD && tmp->token != QUOTE_CMD && tmp->token != DOL)
		return (free_error(var, E_syntax, tmp->word, -2), -1);
	return (0);
}

t_word	*trim_rout(t_word *start, t_var *var)
{
	t_word	*tmp;

	tmp = skip_space(start->next->next);
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
		tmp = skip_space(start->next);
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

void	check_syntax(t_var *var)
{
	if (check_syntax_pipe(var->lexer, var) == -1)
		var->error = true;
	else if (check_syntax_redir(var) == -1)
		var->error = true;
}
