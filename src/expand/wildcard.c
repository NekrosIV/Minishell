/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/23 12:48:39 by kasingh           #+#    #+#             */
/*   Updated: 2024/06/08 16:42:33 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef struct index
{
	int	i;
	int	j;
	int	last_star_i;
	int	last_star_j;
}		t_index;

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

int	*init_tab_wildcard(t_word *start, t_var *var)
{
	int	i;
	int	j;
	int	*tab;

	i = 0;
	tab = malloc(sizeof(int) * (ft_strlen_tword(start, start->token) + 1));
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

void	join_t_word(t_word **start, t_word **new)
{
	t_word	*tmp;

	tmp = (*start)->next;
	(*start)->next = *new;
	(*new)->prev = *start;
	while ((*new))
	{
		if ((*new)->next == NULL)
			break ;
		*new = (*new)->next;
	}
	(*new)->next = tmp;
}

int	expand_it(struct dirent *file, t_word **new, t_var *var)
{
	char	*str;

	str = ft_strdup(file->d_name);
	if (!str)
		free_error(var, E_MALLOC, "file_name", 1);
	if (add_word(new, SINGLE_QUOTE, str) == -1)
		free_error(var, E_MALLOC, "add_word", 1);
	str = ft_strdup(" ");
	if (!str)
		free_error(var, E_MALLOC, "add_word", 1);
	add_word(new, SPACES, str);
	return (1);
}

int	join_and_check_err(int i, t_word **start, t_word **new, t_var *var)
{
	if (i > 0)
		join_t_word(start, new);
	if (i > 1 && ((*start)->token == REDIR_IN || (*start)->token == REDIR_OUT
			|| (*start)->token == REDIR_APPEND))
	{
		var->error = true;
		free_error(NULL, E_REDIR, "*", -1);
		return (-1);
	}
	return (i);
}

int	expand_wildcard(t_word **start, char *pattern, int *tab, t_var *var)
{
	DIR				*dir;
	struct dirent	*file;
	t_word			*new;
	int				i;

	dir = opendir(".");
	if (!dir)
	{
		perror(".");
		var->error = true;
		return (free_error(NULL, NULL, NULL, -1), -1);
	}
	i = 0;
	new = NULL;
	while (dir)
	{
		file = readdir(dir);
		if (!file)
			break ;
		if ((file->d_name[0] != '.' || pattern[0] == '.')
			&& is_expandable(file->d_name, pattern, tab) == true)
			i += expand_it(file, &new, var);
	}
	closedir(dir);
	return (join_and_check_err(i, start, &new, var));
}

void	del_start(t_word **tmp, t_word **start, t_var *var)
{
	*start = *tmp;
	*tmp = (*start)->next;
	if ((*start)->prev == NULL)
		var->lexer = *tmp;
	del_tword(start);
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
