/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_expand.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:29:35 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/27 17:30:09 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_quoted_space(t_var *var, int *i, char *line)
{
	char	*str;
	int		start;

	start = *i;
	(*i)++;
	str = ft_strndup(line, *i, start);
	if (!str)
		free_error(NULL, E_Malloc, "str", 1);
	if (add_word(&var->quoted_cmds, SPACES, str) == -1)
		free_error(NULL, E_Malloc, "add_word", 1);
}

void	handle_quoted_char(t_var *var, int *i, int *tab, char *line)
{
	int		start;
	char	*str;

	start = *i;
	while (tab[(*i)] == CHAR)
		(*i)++;
	str = ft_strndup(line, *i, start);
	if (!str)
		free_error(var, E_Malloc, "str", 1);
	if (add_word(&var->quoted_cmds, CMD, str) == -1)
		free_error(var, E_Malloc, "add_word", 1);
}

void	handle_quoted_dol(t_var *var, int *i, int *tab, char *line)
{
	char	*str;
	int		start;
	int		token;

	start = *i;
	(*i)++;
	token = -1;
	if (tab[start + 1] == CHAR)
	{
		while (tab[(*i)] == CHAR)
			(*i)++;
		token = DOL;
	}
	else
		token = CHAR;
	str = ft_strndup(line, *i, start);
	if (!str)
		free_error(NULL, E_Malloc, "str", 1);
	if (add_word(&var->quoted_cmds, token, str) == -1)
		free_error(NULL, E_Malloc, "add_word", 1);
}

void	handle_quoted_end(t_var *var)
{
	char	*str;

	str = ft_strdup("newline");
	if (!str)
		free_error(NULL, E_Malloc, "str", 1);
	if (add_word(&var->quoted_cmds, END, str) == -1)
		free_error(NULL, E_Malloc, "add_word", 1);
}

void	handle_quoted_token(t_var *var, int *tab, int i, char *line)
{
	while (tab[i] != END)
	{
		if (tab[i] == SPACES)
			handle_quoted_space(var, &i, line);
		else if (tab[i] == DOL)
			handle_quoted_dol(var, &i, tab, line);
		else
			handle_quoted_char(var, &i, tab, line);
	}
	handle_quoted_end(var);
	free(tab);
}
