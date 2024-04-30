/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/22 17:17:46 by pscala            #+#    #+#             */
/*   Updated: 2024/04/30 16:38:30 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_cmd(t_word *node)
{
	int	i;
	int	flag;

	i = 0;
	flag = 1;
	while (node->token != END && node->token != PIPE)
	{
		if (node->token == CMD || node->token == DOUBLE_QUOTE || node->token == SINGLE_QUOTE)
		{
			if (node->prev == NULL)
				i++;
			else if (flag == 0)
			{
				i++;
				flag = 1;
			}
		}
		else
			flag = 0;
		node = node->next;
	}
	return (i);
}

int	ft_strlen_node(t_word *word)
{
	int	i;
	int	j;

	i = 0;
	while (word->token == CMD || word->token == DOUBLE_QUOTE || word->token == SINGLE_QUOTE)
	{
		j = 0;
		while (word->word[j])
		{
			i++;
			j++;
		}
		word = word->next;
	}
	return (i);
}

char	*ft_strjoin_node(t_word *word)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	str = malloc(sizeof(char) * ft_strlen_node(word) + 1);
	if (!str)
		return (NULL);
	while (word->token == CMD || word->token == DOUBLE_QUOTE || word->token == SINGLE_QUOTE)
	{
		j = 0;
		while (word->word[j])
		{
			str[i] = word->word[j];
			i++;
			j++;
		}
		word = word->next;
	}
	str[i] = '\0';
	return (str);
}

t_word	*next_word(t_word *word)
{
	while (word->token == CMD || word->token == DOUBLE_QUOTE || word->token == SINGLE_QUOTE)
		word = word->next;
	return (word);
}

char	**split_cmd(t_var *var)
{
	t_word	*word;
	char	**cmd;
	int		i;

	i = 0;
	word = var->lexer;
	cmd = malloc(sizeof(char *) * (count_cmd(word) + 1));
	if (!cmd)
		return (NULL);
	while (word->token != END && word->token != PIPE)
	{
		if (word->token == CMD || word->token == DOUBLE_QUOTE || word->token == SINGLE_QUOTE)
		{
			cmd[i] = ft_strjoin_node(word);
			if (!cmd[i])
				return (NULL);
			i++;
			word = next_word(word);
		}
		else
			word = word->next;
	}
	cmd[i] = NULL;
	return (cmd);
}
