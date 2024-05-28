/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 16:57:26 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/28 15:26:29 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strndup(char *line, int end, int start)
{
	char	*str;
	int		j;
	int		size;

	size = end - start;
	j = 0;
	str = malloc(sizeof(char) * size + 1);
	if (!str)
		return (NULL);
	while (j < size)
	{
		str[j] = line[start];
		j++;
		start++;
	}
	str[j] = '\0';
	return (str);
}

t_word	*get_last_tword(t_word *word)
{
	if (!word)
		return (NULL);
	while (word->next)
		word = word->next;
	return (word);
}

void	del_tword(t_word **word)
{
	t_word	*temp;
	t_word	*prev;

	temp = *word;
	*word = (*word)->next;
	if (temp->prev == NULL)
		prev = NULL;
	else
	{
		prev = temp->prev;
		prev->next = (*word);
	}
	(*word)->prev = prev;
	free(temp->word);
	free(temp);
}

t_word	*skip_token(t_word *tmp, int token, bool dir)
{
	if (dir == true)
	{
		while (tmp)
		{
			if (tmp->token == token)
				tmp = tmp->next;
			else
				break ;
		}
	}
	else
	{
		while (tmp)
		{
			if (tmp->token == token)
				tmp = tmp->prev;
			else
				break ;
		}
	}
	return (tmp);
}

int	ft_isgoodchar(char c)
{
	if (c == '_' || c == '?' || ft_isalnum(c))
		return (1);
	return (0);
}
