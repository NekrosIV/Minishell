/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 13:57:03 by pscala            #+#    #+#             */
/*   Updated: 2024/04/25 17:57:20 by pscala           ###   ########.fr       */
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

void	expand(t_var *var)
{
	t_word	*tmp;

	tmp = var->lexer;
	while (tmp)
	{
		if (tmp->token == DOL)
			find_and_replace(tmp, var);
		tmp = tmp->next;
	}
}
