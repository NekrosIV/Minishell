/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 13:57:03 by pscala            #+#    #+#             */
/*   Updated: 2024/04/27 17:32:05 by kasingh          ###   ########.fr       */
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

void	init_quoted_cmd(t_word *tmp, t_var *var)
{
	int	*tab;
	int	i;

	i = 0;
	tab = malloc(sizeof(int) * (ft_strlen(tmp->word) + 1));
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
