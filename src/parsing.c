/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 17:19:10 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/13 11:06:33 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	add_word(t_word **word, int token, char *str)
{
	t_word	*new_word;
	t_word	*last_word;

	new_word = malloc(sizeof(t_word));
	if (!new_word)
	{
		printf("Error: malloc failed\n");
		exit(1);
	}
	new_word->word = str;
	new_word->token = token;
	new_word->next = NULL;
	if (!(*word))
	{
		*word = new_word;
		new_word->prev = NULL;
	}
	else
	{
		last_word = *word;
		while (last_word->next)
			last_word = last_word->next;
		last_word->next = new_word;
		new_word->prev = last_word;
	}
}

void	handle_pipe(t_word **word, char *line, int *i)
{
	char	*str;
	int		start;

	start = *i;
	(*i)++;
	str = ft_strndup(line, *i, start);
	if (!str)
	{
		printf("Error: malloc failed\n");
		exit(1);
	}
	add_word(word, PIPE, str);
}

void	handle_redir_in(t_word **word, char *line, int *i)
{
	char	*str;
	int		start;
	int		token;

	start = *i;
	if (line[start + 1] == '<')
	{
		(*i) += 2;
		token = HERE_DOC;
	}
	else
	{
		(*i)++;
		token = REDIR_IN;
	}
	str = ft_strndup(line, i, start);
	if (!str)
	{
		printf("Error: malloc failed\n");
		exit(1);
	}
	add_word(word, token, str);
}

void	handle_redir_out(t_word **word, char *line, int *i)
{
	char	*str;
	int		start;
	int		token;

	start = *i;
	if (line[start + 1] == '>')
	{
		(*i) += 2;
		token = REDIR_APPEND;
	}
	else
	{
		(*i)++;
		token = REDIR_OUT;
	}
	str = ft_strndup(line, i, start);
	if (!str)
	{
		printf("Error: malloc failed\n");
		exit(1);
	}
	add_word(word, token, str);
}

void	parsing(t_var *var)
{
	int i;
	int *tab;
	t_word *word;

	i = 0;
	word = NULL;
	tab = malloc(sizeof(int) * (strlen(var->line) + 1));
	if (!tab)
	{
		printf("Error: malloc failed\n");
		exit(1);
	}
	while (var->line[i])
	{
		if (var->line[i] == '|')
			tab[i] = PIPE;
		else if (var->line[i] == '<')
			tab[i] = REDIR_IN;
		else if (var->line[i] == '>')
			tab[i] = REDIR_OUT;
		else if (var->line[i] == ' ')
			tab[i] = SPACES;
		else if (var->line[i] == '\'')
			tab[i] = SINGLE_QUOTE;
		else if (var->line[i] == '\"')
			tab[i] = DOUBLE_QUOTE;
		else
			tab[i] = CHAR;
		i++;
	}
	tab[i] = END;
	i = 0;
	while (tab[i] != END)
	{
		if (tab[i] == PIPE)
			handle_pipe(&word, var->line, &i);
		else if (tab[i] == REDIR_IN)
			handle_redir_in(&word, var->line, &i);
		else if (tab[i] == REDIR_OUT)
			handle_redir_out(&word, var->line, &i);
		else if (tab[i] == SPACES)
			handle_spaces(&word, var->line, &i);
		else if (tab[i] == SINGLE_QUOTE)
			handle_single_quote(&word, var->line, &i);
		else if (tab[i] == DOUBLE_QUOTE)
			handle_double_quote(&word, var->line, &i);
		else
			handle_char(&word, var->line, &i);
		i++;
	}
	// if (tab[i] == END)
	// 	printf("END\n");
	free(tab);
}