/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 18:19:02 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/07 13:37:13 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
		else if (word->token == CMD)
			printf("token: CMD\n");
		else if (word->token == SPACES)
			printf("token: SPACE\n");
		else if (word->token == SINGLE_QUOTE)
			printf("token: SINGLE_QUOTE\n");
		else if (word->token == DOUBLE_QUOTE)
			printf("token: DOULBLE_QUOTE\n");
		else if (word->token == DOL)
			printf("token: DOL\n");
		else if (word->token == AND)
			printf("token: AND\n");
		else if (word->token == OR)
			printf("token: OR\n");
		else if (word->token == PARENTH_OPEN)
			printf("token: PARENTH_OPEN\n");
		else if (word->token == PARENTH_CLOSE)
			printf("token: PARENTH_CLOSE\n");
		else if (word->token == END)
			printf("token: END\n");
		word = word->next;
	}
}

void	print_list_env(t_env *env)
{
	while (env)
	{
		printf("%s\n", env->line);
		env = env->next;
	}
}
