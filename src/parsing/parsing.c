/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 17:19:10 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/07 13:37:13 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_word(t_word **word, int token, char *str)
{
	t_word	*new_word;
	t_word	*last_word;

	new_word = malloc(sizeof(t_word));
	if (!new_word)
		return (-1);
	new_word->here_doc_expand = true;
	new_word->word = str;
	new_word->token = token;
	new_word->next = NULL;
	last_word = NULL;
	if (!(*word))
	{
		*word = new_word;
		new_word->prev = NULL;
	}
	else
	{
		last_word = get_last_tword(*word);
		last_word->next = new_word;
		new_word->prev = last_word;
	}
	return (0);
}

void	init_tab_token_2(char *line, int *tab, int *count)
{
	int	i;

	i = *count;
	if (line[i] == '\'')
		tab[i] = SINGLE_QUOTE;
	else if (line[i] == '\"')
		tab[i] = DOUBLE_QUOTE;
	else if (line[i] == '$')
		tab[i] = DOL;
	else if (line[i] == '|' && line[i + 1] == '|')
		(tab[i] = OR, i++);
	else if (line[i] == '&' && line[i + 1] == '&')
		(tab[i] = AND, i++);
	else if (line[i] == '(')
		tab[i] = PARENTH_OPEN;
	else if (line[i] == ')')
		tab[i] = PARENTH_CLOSE;
	else
		tab[i] = CHAR;
	*count = i;
}

int	*init_tab_token(char *line, int i)
{
	int	*tab;

	tab = malloc(sizeof(int) * (ft_strlen(line) + 1));
	if (!tab)
		return (NULL);
	while (line[i])
	{
		if (line[i] == '|' && line[i + 1] != '|')
			tab[i] = PIPE;
		else if (line[i] == '<')
			tab[i] = REDIR_IN;
		else if (line[i] == '>')
			tab[i] = REDIR_OUT;
		else if (line[i] == ' ' || line[i] == '\t')
			tab[i] = SPACES;
		else
			init_tab_token_2(line, tab, &i);
		i++;
	}
	tab[i] = END;
	return (tab);
}

void	parsing(t_var *var)
{
	int	i;
	int	*tab;

	i = 0;
	tab = init_tab_token(var->line, i);
	if (!tab)
		free_error(var, E_MALLOC, "tab", 1);
	handle_token(var, tab, i);
	free(tab);
	if (var->error == false)
	{
		count_node(var->lexer);
		print_list(var->lexer);
	}
	if (var->error == false)
		check_syntax(var);
	if (var->error == false)
	{
		count_node(var->lexer);
		print_list(var->lexer);
	}
	// var->error = true;
}
