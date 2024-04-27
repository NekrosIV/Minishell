/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 16:57:59 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/27 19:09:56 by kasingh          ###   ########.fr       */
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
