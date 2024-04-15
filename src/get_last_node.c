/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_last_node.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/13 13:33:58 by pscala            #+#    #+#             */
/*   Updated: 2024/04/13 13:35:52 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_word	*get_last_tword(t_word *word)
{
	if (!word)
		return (NULL);
	while (word->next)
		word = word->next;
	return (word);
}
