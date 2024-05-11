/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_one.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 16:53:20 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/11 13:44:01 by kasingh          ###   ########.fr       */
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

void	handle_quotes(t_var *var, int *i, int token)
{
	int		start;
	char	*str;

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
		str = ft_strndup(var->line, *i, start + 1);
		if (!str)
			free_error(var, E_MALLOC, "str", 1);
		(*i)++;
		if (add_word(&var->lexer, token, str) == -1)
			free_error(var, E_MALLOC, "add_word", 1);
	}
}

void	handle_char(t_var *var, int *i, int *tab)
{
	int		start;
	char	*str;

	start = *i;
	while (tab[(*i)] == CHAR)
		(*i)++;
	str = ft_strndup(var->line, *i, start);
	if (!str)
		free_error(var, E_MALLOC, "str", 1);
	if (add_word(&var->lexer, CMD, str) == -1)
		free_error(var, E_MALLOC, "add_word", 1);
}
