/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 18:19:02 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/18 14:29:56 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_var(t_var *var, t_env **envs)
{
	var->exit = false;
	var->error = false;
	var->line = NULL;
	var->lexer = NULL;
	var->env = *envs;
}

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
		else if (word->token == QUOTE_CMD)
			printf("token: QUOTE_CMD\n");
		else if (word->token == DOL)
			printf("token: DOL\n");
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

// int	prevs_backu_slashu(char *line, int end)
// {
// 	int	i;
// 	int	count;

// 	if (!line)
// 		return (0);
// 	i = 0;
// 	count = 0;
// 	while (i < end)
// 	{
// 		if (line[i] == '\\')
// 			count++;
// 		i++;
// 	}
// 	return (count);
// }
