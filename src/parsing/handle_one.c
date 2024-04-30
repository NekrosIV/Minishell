/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_one.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 16:53:20 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/30 16:35:02 by kasingh          ###   ########.fr       */
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
			free_error(NULL, E_SYNTAX, "\'", -99);
		else
			free_error(NULL, E_SYNTAX, "\"", -99);
	}
	else
	{
		if(var->line[start] == '\'')
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
		else
			handle_char(var, &i, tab);
	}
	handle_end(var);
}
