/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 20:11:58 by pscala            #+#    #+#             */
/*   Updated: 2024/06/08 20:16:37 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
