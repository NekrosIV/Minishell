/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_one.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 16:53:20 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/09 13:09:15 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_pipe(t_var *var, int *i)
{
	char	*str;
	int		start;

	start = *i;
	(*i)++;
	str = ft_strndup(var->line, *i, start);
	if (!str)
		free_error(var, E_MALLOC, "str", 1);
	if (add_word(&var->lexer, PIPE, str) == -1)
		free_error(var, E_MALLOC, "add_word", 1);
}

void	handle_redir_in(t_var *var, int *i)
{
	char	*str;
	int		start;
	int		token;

	start = *i;
	if (var->line[start + 1] == '<')
	{
		(*i)++;
		token = HERE_DOC;
	}
	else
		token = REDIR_IN;
	(*i)++;
	str = ft_strndup(var->line, *i, start);
	if (!str)
		free_error(var, E_MALLOC, "str", 1);
	if (add_word(&var->lexer, token, str) == -1)
		free_error(var, E_MALLOC, "add_word", 1);
}

void	handle_redir_out(t_var *var, int *i)
{
	char	*str;
	int		start;
	int		token;

	start = *i;
	if (var->line[start + 1] == '>')
	{
		(*i)++;
		token = REDIR_APPEND;
	}
	else
		token = REDIR_OUT;
	(*i)++;
	str = ft_strndup(var->line, *i, start);
	if (!str)
		free_error(var, E_MALLOC, "str", 1);
	if (add_word(&var->lexer, token, str) == -1)
		free_error(var, E_MALLOC, "add_word", 1);
}

void	handle_quotes(t_var *var, int *i)
{
	int		start;
	char	*str;
	int		token;

	start = *i;
	(*i)++;
	while (var->line[(*i)] && var->line[(*i)] != var->line[start])
		(*i)++;
	if (var->line[(*i)] != var->line[start])
	{
		var->error = true;
		if (var->line[start] == '\'')
			free_error(NULL, E_EOF, "\'", -2);
		else
			free_error(NULL, E_EOF, "\"", -2);
		free_error(NULL, E_S_EOF, NULL, -2);
	}
	else
	{
		if (var->line[start] == '\'')
			token = SINGLE_QUOTE;
		else
			token = DOUBLE_QUOTE;
		str = ft_strndup(var->line, *i, start + 1);
		if (!str)
			free_error(var, E_MALLOC, "str", 1);
		(*i)++;
		if (add_word(&var->lexer, token, str) == -1)
			free_error(var, E_MALLOC, "add_word", 1);
	}
}

void	handle_or_and(t_var *var, int *i, int *tab)
{
	char	*str;
	int		start;
	int		token;

	start = *i;
	if (tab[start] == OR)
		token = OR;
	else
		token = AND;
	(*i) += 2;
	str = ft_strndup(var->line, *i, start);
	if (!str)
		free_error(var, E_MALLOC, "str", 1);
	if (add_word(&var->lexer, token, str) == -1)
		free_error(var, E_MALLOC, "add_word", 1);
}

void	handle_parent(t_var *var, int *i, int *tab)
{
	char	*str;
	int		start;
	int		token;

	start = *i;
	if (tab[start] == PARENTH_OPEN)
		token = PARENTH_OPEN;
	else
		token = PARENTH_CLOSE;
	(*i)++;
	str = ft_strndup(var->line, *i, start);
	if (!str)
		free_error(var, E_MALLOC, "str", 1);
	if (add_word(&var->lexer, token, str) == -1)
		free_error(var, E_MALLOC, "add_word", 1);
}

void	handle_token(t_var *var, int *tab, int i)
{
	while (tab[i] != END)
	{
		if (tab[i] == PIPE)
			handle_pipe(var, &i);
		else if (tab[i] == REDIR_IN)
			handle_redir_in(var, &i);
		else if (tab[i] == REDIR_OUT)
			handle_redir_out(var, &i);
		else if (tab[i] == SINGLE_QUOTE || tab[i] == DOUBLE_QUOTE)
			handle_quotes(var, &i);
		else if (tab[i] == SPACES)
			handle_space(var, &i);
		else if (tab[i] == DOL)
			handle_dol(var, &i, tab);
		else if (tab[i] == OR || tab[i] == AND)
			handle_or_and(var, &i, tab);
		else if (tab[i] == PARENTH_OPEN || tab[i] == PARENTH_CLOSE)
			handle_parent(var, &i, tab);
		else
			handle_char(var, &i, tab);
	}
	handle_end(var);
}
