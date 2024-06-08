/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 20:20:04 by pscala            #+#    #+#             */
/*   Updated: 2024/06/08 20:20:42 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	*init_tab_wildcard(t_word *start, t_var *var)
{
	int	i;
	int	j;
	int	*tab;

	i = 0;
	tab = malloc(sizeof(int) * (ft_strlen_tword(start, start->token) + 1));
	if (!tab)
		free_error(var, E_MALLOC, "str", 1);
	while (start->token != END && is_cmd(start->token) == 1)
	{
		j = 0;
		while (start->word[j])
		{
			if (start->in_quote == false && start->word[j] == '*')
				tab[i] = WILDCARD;
			else
				tab[i] = CHAR;
			j++;
			i++;
		}
		start = start->next;
	}
	tab[i] = END;
	return (tab);
}
