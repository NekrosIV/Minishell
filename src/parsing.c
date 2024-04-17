/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 17:19:10 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/17 17:50:52 by pscala           ###   ########.fr       */
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

void	handle_pipe(t_var *var, int *i)
{
	char	*str;
	int		start;

	start = *i;
	(*i)++;
	str = ft_strndup(var->line, *i, start);
	if (!str)
		free_error(var, E_Malloc, "str", 1);
	if (add_word(&var->lexer, PIPE, str) == -1)
		free_error(var, E_Malloc, "add_word", 1);
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
		free_error(var, E_Malloc, "str", 1);
	if (add_word(&var->lexer, token, str) == -1)
		free_error(var, E_Malloc, "add_word", 1);
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
		free_error(var, E_Malloc, "str", 1);
	if (add_word(&var->lexer, token, str) == -1)
		free_error(var, E_Malloc, "add_word", 1);
}

// void	handle_back_slash(t_var *var, int *i)
// {
// 	int		start;
// 	char	*str;

// 	start = *i;
// 	str = NULL;
// 	if (start == 0)
// 		(*i)++;
// 	else if (var->line[start - 1] && prevs_backu_slashu(var->line, start)
// % 2 == 0)
// 		(*i)++;
// 	else
// 	{
// 		(*i)++;
// 		str = ft_strndup(var->line, *i, start);
// 		if (!str)
// 		{
// 			printf("Error: malloc failed\n");
// 			exit(1);
// 		}
// 		add_word(&var->lexer, CMD, str);
// 	}
// }

void	handle_char(t_var *var, int *i, int *tab)
{
	int		start;
	char	*str;

	start = *i;
	while (tab[(*i)] == CHAR)
		(*i)++;
	str = ft_strndup(var->line, *i, start);
	if (!str)
		free_error(var, E_Malloc, "str", 1);
	if (add_word(&var->lexer, CMD, str) == -1)
		free_error(var, E_Malloc, "add_word", 1);
}

void	handle_quotes(t_var *var, int *i)
{
	int		start;
	char	*str;
	int		flag;

	start = *i;
	(*i)++;
	while (var->line[(*i)] && var->line[(*i)] != var->line[start])
		(*i)++;
	if (var->line[(*i)] != var->line[start])
	{
		var->error = true;
		free_error(NULL, "Error: quotes : ", "\"", -99);
	}
	else
	{
		printf("else\n");
		str = ft_strndup(var->line, *i, start + 1);
		if (!str)
			free_error(var, E_Malloc, "str", 1);
		(*i)++;
		if (add_word(&var->lexer, QUOTE_CMD, str) == -1)
			free_error(var, E_Malloc, "add_word", 1);
	}
}

void	handle_space(t_var *var, int *i)
{
	char	*str;
	int		start;

	start = *i;
	(*i)++;
	str = ft_strndup(var->line, *i, start);
	if (!str)
		free_error(NULL, E_Malloc, "str", 1);
	if (add_word(&var->lexer, SPACES, str) == -1)
		free_error(NULL, E_Malloc, "add_word", 1);
}

void	parsing(t_var *var)
{
	int	i;
	int	*tab;

	i = 0;
	tab = malloc(sizeof(int) * (strlen(var->line) + 1));
	if (!tab)
		free_error(var, E_Malloc, "var", 1);
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
			handle_pipe(var, &i);
		else if (tab[i] == REDIR_IN)
			handle_redir_in(var, &i);
		else if (tab[i] == REDIR_OUT)
			handle_redir_out(var, &i);
		else if (tab[i] == SINGLE_QUOTE || tab[i] == DOUBLE_QUOTE)
			handle_quotes(var, &i);
		else if (tab[i] == SPACES)
			handle_space(var, &i);
		else
			handle_char(var, &i, tab);
	}
	free(tab);
	if (var->error == false)
	{
		count_node(var->lexer);
		print_list(var->lexer);
	}
}
