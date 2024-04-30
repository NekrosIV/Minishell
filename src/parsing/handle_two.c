/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_two.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 16:53:17 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/30 15:55:56 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

void	handle_space(t_var *var, int *i)
{
	char	*str;
	int		start;

	start = *i;
	(*i)++;
	str = ft_strndup(var->line, *i, start);
	if (!str)
		free_error(NULL, E_MALLOC, "str", 1);
	if (add_word(&var->lexer, SPACES, str) == -1)
		free_error(NULL, E_MALLOC, "add_word", 1);
}

void	handle_dol(t_var *var, int *i, int *tab)
{
	char	*str;
	int		start;
	int		token;

	start = *i;
	(*i)++;
	token = -1;
	if (tab[start + 1] == CHAR && ft_isgoodchar(var->line[(*i)]) == 1)
	{
		while (tab[(*i)] == CHAR && ft_isgoodchar(var->line[(*i)]) == 1)
			(*i)++;
		token = DOL;
	}
	else if (tab[start + 1] != DOUBLE_QUOTE && tab[start + 1] != SINGLE_QUOTE)
		token = CMD;
	if (token != -1)
	{
		str = ft_strndup(var->line, *i, start);
		if (!str)
			free_error(NULL, E_MALLOC, "str", 1);
		if (add_word(&var->lexer, token, str) == -1)
			free_error(NULL, E_MALLOC, "add_word", 1);
	}
}

void	handle_end(t_var *var)
{
	char	*str;

	str = ft_strdup("newline");
	if (!str)
		free_error(NULL, E_MALLOC, "str", 1);
	if (add_word(&var->lexer, END, str) == -1)
		free_error(NULL, E_MALLOC, "add_word", 1);
}
