/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 13:57:03 by pscala            #+#    #+#             */
/*   Updated: 2024/06/09 15:44:56 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_dol(t_word *tmp, char *str)
{
	free(tmp->word);
	tmp->word = str;
	if (!tmp->word)
		free_error(NULL, E_MALLOC, "find_and_replace", 1);
	tmp->token = CMD;
}

char	*find_in_env(char *str, t_var *var)
{
	t_env	*envp;
	int		len;
	char	*result;

	envp = var->env;
	len = ft_strlen(str);
	result = NULL;
	while (envp)
	{
		if (ft_strncmp(&str[1], envp->line, len - 1) == 0 && envp->line[len
			- 1] == '=')
		{
			result = ft_strdup(&envp->line[len]);
			if (!result)
				free_error(var, E_MALLOC, "find_in_env", 1);
		}
		envp = envp->next;
	}
	return (result);
}

void	find_and_replace(t_word *tmp, t_var *var)
{
	char	*str;

	if (ft_strncmp(tmp->word, "$?", 2) == 0)
		replace_dol(tmp, ft_itoa(g_exit_status));
	else
	{
		str = find_in_env(tmp->word, var);
		if (str)
			replace_dol(tmp, str);
	}
}

void	init_quoted_cmd(t_word *tmp, t_var *var)
{
	int	*tab;
	int	i;

	i = 0;
	tab = malloc(sizeof(int) * (ft_strlen(tmp->word) + 1));
	if (!tab)
		free_error(var, E_MALLOC, "tab", 1);
	while (tmp->word[i])
	{
		if (tmp->word[i] == '$')
			tab[i] = DOL;
		else if (!ft_isgoodchar(tmp->word[i]))
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
	t_word	*tmp2;

	tmp = var->lexer;
	tmp2 = NULL;
	while (tmp)
	{
		if (tmp->token == DOL)
			find_and_replace(tmp, var);
		else if (tmp->token == DOUBLE_QUOTE && ft_strchr(tmp->word, '$'))
			init_quoted_cmd(tmp, var);
		if (tmp->token == DOL)
		{
			tmp2 = tmp->next;
			del_tword(&tmp);
			tmp = tmp2;
			if (!tmp->prev)
				var->lexer = tmp;
		}
		else
			tmp = tmp->next;
	}
}
