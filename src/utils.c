/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 18:19:02 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/13 15:43:23 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_var(t_var *var)
{
	var->line = NULL;
	var->lexer = NULL;
}

char	*ft_strndup(char *line, int i, int start)
{
	char	*str;
	int		j;
	int		size;

	size = i - start;
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

void	count_node(t_word *word)
{
	int	i;

	i = 0;
	while (word)
	{
		i++;
		word = word->next;
	}
	printf("nombre de noeud %d\n", i);
}
void	print_list(t_word *word)
{
	while (word)
	{
		printf("word: %s ", word->word);
		if (word->token == PIPE)
			printf("token: PIPE\n");
		else if (word->token == REDIR_IN)
			printf("token: REDIR_IN\n");
		else if (word->token == REDIR_OUT)
			printf("token: REDIR_OUT\n");
		else if (word->token == HERE_DOC)
			printf("token: HERE_DOC\n");
		else if (word->token == REDIR_APPEND)
			printf("token: REDIR_APPEND\n");
		word = word->next;
	}
}
