/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 13:57:03 by pscala            #+#    #+#             */
/*   Updated: 2024/04/26 19:05:49 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_dol(t_word *tmp, char *str)
{
	free(tmp->word);
	tmp->word = str;
	if (!tmp->word)
		free_error(NULL, E_Malloc, "find_and_replace", 1);
	tmp->token = CMD;
}

void	find_and_replace(t_word *tmp, t_var *var)
{
	t_env	*envp;
	int		len;

	envp = var->env;
	len = ft_strlen(tmp->word);
	if (ft_strncmp(tmp->word, "$?", 2) == 0)
		replace_dol(tmp, ft_itoa(var->status));
	else
	{
		while (envp)
		{
			if (ft_strncmp(&tmp->word[1], envp->line, len - 1) == 0
				&& envp->line[len - 1] == '=')
				replace_dol(tmp, ft_strdup(&envp->line[len]));
			envp = envp->next;
		}
	}
}

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
}

void	expand_quoted_cmds(t_var *var)
{
	t_word	*tmp;

	tmp = var->quoted_cmds;
	while (tmp)
	{
		if (tmp->token == DOL)
			find_and_replace(tmp, var);
		tmp = tmp->next;
	}
}

int	len_quoted_cmd(t_var *var)
{
	int		i;
	int		j;
	t_word	*tmp;

	i = 0;
	j = 0;
	tmp = var->quoted_cmds;
	while (tmp->token != END)
	{
		j = 0;
		if (tmp->token != DOL)
		{
			while (tmp->word[j])
			{
				j++;
				i++;
			}
		}
		tmp = tmp->next;
	}
	return (i);
}

void	fill_tab(t_var *var, char **word)
{
	int		i;
	int		j;
	t_word	*tmp;

	i = 0;
	j = 0;
	tmp = var->quoted_cmds;
	while (tmp->token != END)
	{
		j = 0;
		if (tmp->token != DOL)
		{
			while (tmp->word[j])
			{
				(*word)[i] = tmp->word[j];
				j++;
				i++;
			}
		}
		tmp = tmp->next;
	}
	(*word)[i] = '\0';
}

char	*join_quoted_cmd(t_var *var)
{
	char	*word;
	int		len;

	len = len_quoted_cmd(var);
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		free_error(var, E_Malloc, "tab joined quote", 1);
	fill_tab(var, &word);
	return (word);
}

void	init_quoted_cmd(t_word *tmp, t_var *var)
{
	int	*tab;
	int	i;

	i = 0;
	tab = malloc(sizeof(int) * (strlen(tmp->word) + 1));
	if (!tab)
		free_error(var, E_Malloc, "tab", 1);
	while (tmp->word[i])
	{
		if (tmp->word[i] == '$')
			tab[i] = DOL;
		else if (tmp->word[i] == ' ')
			tab[i] = SPACES;
		else
			tab[i] = CHAR;
		i++;
	}
	tab[i] = END;
	i = 0;
	handle_quoted_token(var, tab, i, tmp->word);
	expand_quoted_cmds(var);
	free(tmp->word);
	tmp->word = join_quoted_cmd(var);
	free_list_lexer(&var->quoted_cmds);
}

void	expand(t_var *var)
{
	t_word	*tmp;

	tmp = var->lexer;
	while (tmp)
	{
		if (tmp->token == DOL)
			find_and_replace(tmp, var);
		else if (tmp->token == QUOTE_CMD && ft_strchr(tmp->word, '$'))
			init_quoted_cmd(tmp, var);
		tmp = tmp->next;
	}
}
