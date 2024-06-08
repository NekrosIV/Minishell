/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   before_exec_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 20:09:19 by pscala            #+#    #+#             */
/*   Updated: 2024/06/08 20:09:43 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	join_node2(t_word *head, t_var *var)
{
	t_word	*end;
	char	*str;
	t_word	*start;
	t_word	*temp;

	if (head->next == NULL)
		return ;
	end = head->next;
	start = head->next;
	while (end->token == CMD || end->token == DOUBLE_QUOTE
		|| end->token == SINGLE_QUOTE)
		end = end->next;
	if (start == end)
		return ;
	str = ft_strjoin_tword(head, var, head->token);
	(free(head->word), head->word = str);
	while (start != end)
	{
		temp = start->next;
		(del_tword(&start), start = temp);
	}
}

void	join_node(t_var *var)
{
	t_word	*tmp;

	tmp = var->lexer;
	while (tmp->token != END)
	{
		if (is_cmd(tmp->token) == 1)
			join_node2(tmp, var);
		tmp = tmp->next;
	}
}
