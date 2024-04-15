/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 17:19:10 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/15 18:15:42 by pscala           ###   ########.fr       */
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
}

void	handle_pipe(t_word **word, char *line, int *i)
{
	char	*str;
	int		start;
	int		token;

	start = *i;
	if (line[start - 1] == '\\')
		token = CHAR;
	else
		token = PIPE;
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
	if (line[start - 1] == '\\')
		token = CMD;
	else if (line[start + 1] == '<')
	{
		(*i)++;
		token = HERE_DOC;
	}
	else
		token = REDIR_IN;
	(*i)++;
	str = ft_strndup(line, *i, start);
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
	if (line[start - 1] == '\\')
		token = CMD;
	else if (line[start + 1] == '>')
	{
		(*i)++;
		token = REDIR_APPEND;
	}
	else
		token = REDIR_OUT;
	(*i)++;
	str = ft_strndup(line, *i, start);
	if (!str)
	{
		printf("Error: malloc failed\n");
		exit(1);
	}
	add_word(word, token, str);
}

void	handle_back_slash(t_word **word, char *line, int *i)
{
	int		start;
	char	*str;

	start = *i;
	str = NULL;
	if (start == 0)
		(*i)++;
	else if (line[start - 1] && prevs_backu_slashu(line, start) % 2 == 0)
		(*i)++;
	else
	{
		(*i)++;
		str = ft_strndup(line, *i, start);
		if (!str)
		{
			printf("Error: malloc failed\n");
			exit(1);
		}
		add_word(word, CMD, str);
	}
}

void	handle_char(t_word **word, char *line, int *i, int *tab)
{
	int		start;
	char	*str;

	start = *i;
	while (tab[(*i)] == CHAR)
		(*i)++;
	str = ft_strndup(line, *i, start);
	if (!str)
	{
		printf("Error: malloc failed\n");
		exit(1);
	}
	add_word(word, CMD, str);
}

void	handle_quotes(t_word **word, char *line, int *i)
{
	int		start;
	char	*str;
	int		flag;

	start = *i;
	(*i)++;
	while (line[(*i)])
	{
		if (line[(*i)] == line[start] && line[(*i) - 1] != '\\')
			break ;
		(*i)++;
	}
	if (line[(*i)] != line[start])
		printf("Error: quotes\n");
	else
	{
		str = ft_strndup(line, *i, start + 1);
		if (!str)
		{
			printf("Error: malloc failed\n");
			exit(1);
		}
		add_word(word, CMD, str);
	}
}

void	handle_space(t_word **word, char *line, int *i)
{
	char	*str;
	int		start;
	int		token;

	start = *i;
	if (start == 0)
		(*i)++;
	else if (line[start - 1] && prevs_backu_slashu(line, start) % 2 != 0)
		token = CMD;
	else
		token = SPACES;
	(*i)++;
	str = ft_strndup(line, *i, start);
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
		else if (var->line[i] == '\\')
			tab[i] = BACKU_SLASHU;
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
		else if (tab[i] == BACKU_SLASHU)
			handle_back_slash(&word, var->line, &i);
		else if (tab[i] == SINGLE_QUOTE || tab[i] == DOUBLE_QUOTE)
			handle_quotes(&word, var->line, &i);
		else if (tab[i] == SPACES)
			handle_space(&word, var->line, &i);
		else
			handle_char(&word, var->line, &i, tab);
		// i++;
	}
	if (tab[i] == END)
		printf("END\n");
	count_node(word);
	print_list(word);
	free(tab);
}