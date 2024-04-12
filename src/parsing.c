/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 17:19:10 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/11 13:48:39 by kasingh          ###   ########.fr       */
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
	int		j;

	j = *i;
	str = malloc(sizeof(char) * 2);
	if (!str)
	{
		printf("Error: malloc failed\n");
		exit(1);
	}
	str[0] = line[j];
	str[1] = '\0';
	add_word(word, PIPE, str);
	(*i)++;
}

void	handle_redir_in(t_word **word, char *line, int *i)
{
	char	*str;
	int		j;

	j = *i;
	if (line[j + 1] == '<')
	{
		str = malloc(sizeof(char) * 3);
		if (!str)
		{
			printf("Error: malloc failed\n");
			exit(1);
		}
		str[0] = line[j];
		str[1] = line[j + 1];
		str[2] = '\0';
		add_word(word, REDIR_APPEND, str);
		(*i) += 2;
	}
	else
	{
		str = malloc(sizeof(char) * 2);
		if (!str)
		{
			printf("Error: malloc failed\n");
			exit(1);
		}
		str[0] = line[j];
		str[1] = '\0';
		add_word(word, REDIR_IN, str);
		(*i)++;
	}
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
			printf("REDIR_IN\n");
		else if (tab[i] == REDIR_OUT)
			printf("REDIR_OUT\n");
		else if (tab[i] == SPACES)
			printf("SPACES\n");
		else if (tab[i] == SINGLE_QUOTE)
			printf("SINGLE_QUOTE\n");
		else if (tab[i] == DOUBLE_QUOTE)
			printf("DOUBLE_QUOTE\n");
		else
			printf("CHAR\n");
		i++;
	}
	if (tab[i] == END)
		printf("END\n");
	free(tab);
}