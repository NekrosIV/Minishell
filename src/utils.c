/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 18:19:02 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/25 17:29:25 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_var(t_var *var, t_env **envs, int exit_statut)
{
	var->exit = false;
	var->error = false;
	var->line = NULL;
	var->lexer = NULL;
	var->envp = NULL;
	var->env = *envs;
	var->status = exit_statut;
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
int	count_node_env(t_env *env)
{
	int	i;

	i = 0;
	while (env)
	{
		i++;
		env = env->next;
	}
	return (i);
}

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

int	count_node_token(t_word *lexer, int token)
{
	int	i;

	i = 0;
	while (lexer)
	{
		if (lexer->token == token)
			i++;
		lexer = lexer->next;
	}
	return (i);
}
char	**split_env(t_env *env)
{
	char	**result;
	int		i;

	result = malloc(sizeof(char *) * (count_node_env(env) + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (env)
	{
		result[i] = ft_strdup(env->line);
		if (!result[i])
			return (free_split(result), NULL);
		i++;
		env = env->next;
	}
	result[i] = NULL;
	return (result);
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

void	close_fd(int fd, int i)
{
	if (i != 0)
		close(fd);
}

void	del_cmd(t_word **word)
{
	t_word *tmp;

	while ((*word)->token != PIPE && (*word)->token != END)
	{
		tmp = *word;
		(*word) = (*word)->next;
		del_tword(&tmp);
	}
	if ((*word)->token == PIPE)
	{
		tmp = *word;
		(*word) = (*word)->next;
		del_tword(&tmp);
	}
}