/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 12:48:39 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/28 15:52:59 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_word	*find_start_or_end(t_word *tmp, bool dir)
{
	if (dir == true)
	{
		while (tmp->token != END)
		{
			if (is_cmd(tmp->token) == 0)
			{
				tmp = tmp->prev;
				break ;
			}
			tmp = tmp->next;
		}
	}
	else
	{
		while (tmp)
		{
			if (is_cmd(tmp->token) == 0 || tmp->prev == NULL)
			{
				tmp = tmp->next;
				break ;
			}
			tmp = tmp->prev;
		}
	}
	return (tmp);
}

bool	is_simple_wildcard(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != '*')
			return (false);
		i++;
	}
	return (true);
}

int	*init_tab_wildcard(t_word *start, t_var *var)
{
	int	i;
	int	j;
	int	*tab;

	i = 0;
	tab = malloc(sizeof(int) * ft_strlen_tword(start) + 1);
	if (!tab)
		free_error(var, E_MALLOC, "str", 1);
	while (start->token != END && is_cmd(start->token) == 1)
	{
		j = 0;
		while (start->word[j])
		{
			if (start->in_quote == false && start->word[j] == '*')
				tab[i] = WILDCARD;
			else
				tab[i] = CHAR;
			j++;
			i++;
		}
		start = start->next;
	}
	tab[i] = END;
	return (tab);
}
void	del_this_shit(t_word **start, t_word **end)
{
	t_word	*tmp;

	while ((*start) != (*end))
	{
		tmp = (*start)->next;
		(del_tword(start), *start = tmp);
	}
}
void	do_wildcard(t_var *var)
{
	t_word	*tmp;
	t_word	*start;
	t_word	*end;
	int		*tab;
	char	*pattern;

	// t_word	*temp;
	tmp = var->lexer;
	tab = NULL;
	pattern = NULL;
	while (tmp->token != END)
	{
		if (tmp->in_quote == false && ft_strchr(tmp->word, '*') != NULL)
		{
			start = find_start_or_end(tmp, false);
			end = find_start_or_end(tmp, true);
			tab = init_tab_wildcard(start, var);
			pattern = ft_strjoin_tword(start, var, start->token);
			printf("pattern = %s\n", pattern);
			(free(start->word), start->word = pattern);
			if (start != end)
				del_this_shit(&start->next, &end);
		}
		tmp = tmp->next;
	}
}
