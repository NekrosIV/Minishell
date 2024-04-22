/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:17:46 by pscala            #+#    #+#             */
/*   Updated: 2024/04/22 17:55:18 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_cmd(t_word *node)
{
	int	i;

	i = 0;
	while (node->token != END && node->token != PIPE)
	{
		if (node->token == CMD || node->token == QUOTE_CMD)
		{
			if (node->prev == NULL)
				i++;
			else if (node->prev->token != CMD && node->prev->token != QUOTE_CMD)
				i++;
		}
		node = node->next;
	}
	return (i);
}

// char	**split_cmd(t_var *var)
// {
// }