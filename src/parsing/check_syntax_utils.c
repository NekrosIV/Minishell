/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 13:20:36 by kasingh           #+#    #+#             */
/*   Updated: 2024/06/11 18:23:39 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen_tword(t_word *tmp, int token)
{
	int	i;
	int	j;

	i = 0;
	while (tmp->token == CMD || tmp->token == DOUBLE_QUOTE
		|| tmp->token == SINGLE_QUOTE || tmp->token == DOL
		|| token == REDIR_APPEND || token == REDIR_IN || token == REDIR_OUT)
	{
		j = 0;
		while (tmp->word[j])
		{
			i++;
			j++;
		}
		tmp = tmp->next;
		token = -1;
	}
	return (i);
}

void	trim_tword(t_word **start, t_word **end)
{
	t_word	*temp;
	int		token;

	token = (*start)->token;
	while ((*start)->token != (*end)->token && (*start)->token != END)
	{
		temp = *start;
		*start = (*start)->next;
		del_tword(&temp);
	}
	(*end)->token = token;
}

t_word	*trim_rout(t_word *start, t_var *var)
{
	t_word	*tmp;

	tmp = skip_token(start->next->next, SPACES, true);
	if (check_token(tmp, start, var))
		return (NULL);
	trim_tword(&start, &tmp);
	return (tmp);
}

void	handle_token_logic(t_word *tmp, t_var *var, int token, t_word *head)
{
	if (token == -1)
		return ;
	if (token != HERE_DOC)
	{
		if (tmp->token == DOL)
			find_and_replace(tmp, var);
		else if (tmp->token == DOUBLE_QUOTE)
			init_quoted_cmd(tmp, var);
	}
	if (token == HERE_DOC && (tmp->token == DOUBLE_QUOTE
			|| tmp->token == SINGLE_QUOTE))
		head->here_doc_expand = false;
}

bool	can_i_join(t_word *tmp, int token)
{
	return (tmp->token == CMD || tmp->token == DOUBLE_QUOTE
		|| tmp->token == SINGLE_QUOTE || tmp->token == DOL
		|| token == REDIR_APPEND || token == REDIR_IN || token == REDIR_OUT);
}
