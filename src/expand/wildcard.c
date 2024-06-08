/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 12:48:39 by kasingh           #+#    #+#             */
/*   Updated: 2024/06/08 20:20:56 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_word	*find_start(t_word *tmp)
{
	if (tmp->prev == NULL)
		return (tmp);
	while (tmp)
	{
		if (is_cmd(tmp->token) == 0 || tmp->prev == NULL)
			return (tmp->next);
		tmp = tmp->prev;
	}
	return (tmp);
}

void	del_this_shit(t_word **start)
{
	t_word	*tmp;

	while (is_cmd((*start)->token) == 1)
	{
		tmp = (*start)->next;
		(del_tword(start), *start = tmp);
	}
}

void	handle_index(t_index *index, int cas)
{
	if (cas == 1)
	{
		index->last_star_i = index->i++;
		index->last_star_j = index->j;
	}
	else if (cas == 2)
	{
		index->i++;
		index->j++;
	}
	else if (cas == 3)
	{
		index->i = index->last_star_i + 1;
		index->j = ++index->last_star_j;
	}
}

bool	is_expandable(char *file, char *pattern, int *tab)
{
	t_index	index;

	index.i = 0;
	index.j = 0;
	index.last_star_i = -1;
	index.last_star_j = -1;
	while (file[index.j])
	{
		if (pattern[index.i] == '*' && tab[index.i] == WILDCARD)
			handle_index(&index, 1);
		else if (pattern[index.i] == file[index.j])
			handle_index(&index, 2);
		else if (index.last_star_i != -1)
			handle_index(&index, 3);
		else
			return (false);
	}
	while (pattern[index.i] == '*' && tab[index.i] == WILDCARD)
		index.i++;
	return (pattern[index.i] == '\0');
}

void	do_wildcard(t_var *var)
{
	t_word	*tmp;
	t_word	*start;
	int		*tab;
	char	*pattern;

	tmp = var->lexer;
	while (tmp->token != END && var->error == false)
	{
		if (tmp->in_quote == false && ft_strchr(tmp->word, '*') != NULL)
		{
			start = find_start(tmp);
			tmp = start;
			tab = init_tab_wildcard(start, var);
			pattern = ft_strjoin_tword(start, var, start->token);
			(free(start->word), start->word = pattern);
			if (is_cmd(start->next->token) == 1)
				del_this_shit(&start->next);
			if (expand_wildcard(&start, pattern, tab, var) > 0)
				del_start(&tmp, &start, var);
			free(tab);
		}
		tmp = tmp->next;
	}
}
