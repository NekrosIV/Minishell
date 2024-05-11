/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 16:57:59 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/11 15:41:34 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*join_quoted_cmd(t_var *var)
{
	char	*word;
	int		len;

	len = len_quoted_cmd(var);
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		free_error(var, E_MALLOC, "tab joined quote", 1);
	fill_tab(var, &word);
	return (word);
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

char	*expand_here_doc_line(char *line, t_var *var)
{
	char	*expanded_line;
	char	*tmp_line;

	expanded_line = NULL;
	tmp_line = ft_strtrim(line, "\n");
	if (!tmp_line)
	{
		free(line);
		free_error(var, E_MALLOC, "tmp_line", 1);
	}
	expanded_line = find_in_env(tmp_line, var);
	(free(line), free(tmp_line));
	if (!expanded_line)
		line = ft_strdup("\n");
	else
	{
		line = ft_strjoin(expanded_line, "\n");
		free(expanded_line);
	}
	if (!line)
		free_error(var, E_MALLOC, "line", 1);
	return (line);
}
