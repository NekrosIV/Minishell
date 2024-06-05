/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 18:19:02 by kasingh           #+#    #+#             */
/*   Updated: 2024/06/05 14:14:05 by kasingh          ###   ########.fr       */
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
	if (!word)
	{
		printf("no word\n");
		return ;
	}
	while (word)
	{
		if (word->in_quote == true)
			fprintf(stderr, "IN QUOTE ");
		fprintf(stderr, "word: %s ", word->word);
		if (word->token == PIPE)
			fprintf(stderr, "token: PIPE\n");
		else if (word->token == REDIR_IN)
			fprintf(stderr, "token: REDIR_IN\n");
		else if (word->token == REDIR_OUT)
			fprintf(stderr, "token: REDIR_OUT\n");
		else if (word->token == HERE_DOC)
			fprintf(stderr, "token: HERE_DOC\n");
		else if (word->token == REDIR_APPEND)
			fprintf(stderr, "token: REDIR_APPEND\n");
		else if (word->token == CMD)
			fprintf(stderr, "token: CMD\n");
		else if (word->token == SPACES)
			fprintf(stderr, "token: SPACE\n");
		else if (word->token == SINGLE_QUOTE)
			fprintf(stderr, "token: SINGLE_QUOTE\n");
		else if (word->token == DOUBLE_QUOTE)
			fprintf(stderr, "token: DOULBLE_QUOTE\n");
		else if (word->token == DOL)
			fprintf(stderr, "token: DOL\n");
		else if (word->token == AND)
			fprintf(stderr, "token: AND\n");
		else if (word->token == OR)
			fprintf(stderr, "token: OR\n");
		else if (word->token == PARENTH_OPEN)
			fprintf(stderr, "token: PARENTH_OPEN\n");
		else if (word->token == PARENTH_CLOSE)
			fprintf(stderr, "token: PARENTH_CLOSE\n");
		else if (word->token == END)
			fprintf(stderr, "token: END\n");
		word = word->next;
	}
	fprintf(stderr, "\n");
}

void	print_list_env(t_env *env)
{
	while (env)
	{
		fprintf(stderr, "%s\n", env->line);
		env = env->next;
	}
}
