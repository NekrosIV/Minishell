/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_two.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:24:47 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/27 17:25:01 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	node_cmp_token(t_word *lexer, int token)
{
	while (lexer)
	{
		if (lexer->token == token)
			return (1);
		lexer = lexer->next;
	}
	return (0);
}
