/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_redir.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 17:13:12 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/10 14:33:55 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strlen_tword(t_word *tmp)
{
	int	i;
	int	j;

	i = 0;
	while (tmp->token == CMD || tmp->token == DOUBLE_QUOTE
		|| tmp->token == SINGLE_QUOTE || tmp->token == DOL)
	{
		j = 0;
		while (tmp->word[j])
		{
			i++;
			j++;
		}
		tmp = tmp->next;
	}
	return (i);
}

char	*ft_strjoin_tword(t_word *tmp, t_var *var, int token)
{
	char	*str;
	int		i;
	int		j;
	t_word	*head;

	head = tmp;
	str = malloc(sizeof(char) * ft_strlen_tword(tmp) + 1);
	if (!str)
		free_error(var, E_MALLOC, "str", 1);
	i = 0;
	while (tmp->token == CMD || tmp->token == DOUBLE_QUOTE
		|| tmp->token == SINGLE_QUOTE || tmp->token == DOL)
	{
		j = 0;
		if (token != HERE_DOC)
		{
			if (tmp->token == DOL)
				find_and_replace(tmp, var);
			else if (tmp->token == DOUBLE_QUOTE)
				init_quoted_cmd(tmp, var);
		}
		if (token == HERE_DOC && (tmp->token == DOUBLE_QUOTE
				|| tmp->token == SINGLE_QUOTE))
			head->here_doc_expand = false;
		if (tmp->token != DOL || token == HERE_DOC)
		{
			while (tmp->word[j])
			{
				str[i] = tmp->word[j];
				i++;
				j++;
			}
		}
		tmp = tmp->next;
	}
	str[i] = '\0';
	return (str);
}

void	join_if_need(t_word *head, t_var *var, int token)
{
	t_word	*end;
	char	*str;
	t_word	*start;
	int		i;
	t_word	*temp;

	end = head;
	i = 0;
	start = head->next;
	while (end->token == CMD || end->token == DOUBLE_QUOTE
		|| end->token == SINGLE_QUOTE || end->token == DOL)
		(end = end->next, i++);
	if (token == HERE_DOC && (head->token == DOUBLE_QUOTE
			|| head->token == SINGLE_QUOTE))
		head->here_doc_expand = false;
	if (i <= 1)
		return ;
	str = ft_strjoin_tword(head, var, token);
	free(head->word);
	head->word = str;
	while (start != end)
	{
		temp = start->next;
		del_tword(&start);
		start = temp;
	}
}

bool	check_token(t_word *tmp, t_word *start, t_var *var)
{
	if (tmp->token == DOL && start->token != HERE_DOC)
	{
		find_and_replace(tmp, var);
		if (tmp->token == DOL)
			return (free_error(var, E_REDIR, tmp->word, -1), -1);
	}
	join_if_need(tmp, var, start->token);
	if (tmp->token != CMD && tmp->token != DOUBLE_QUOTE
		&& tmp->token != SINGLE_QUOTE && tmp->token != DOL)
		return (free_error(var, E_SYNTAX, tmp->word, -2), -1);
	return (0);
}

t_word	*trim_rout(t_word *start, t_var *var)
{
	t_word	*tmp;

	tmp = skip_token(start->next->next, SPACES, true);
	if (check_token(tmp, start, var))
		return (NULL);
	trim_tword(&start, &tmp);
	return (tmp);
}

t_word	*check_and_trim(t_word *start, t_var *var)
{
	t_word	*tmp;

	tmp = start;
	if (start->token == REDIR_OUT && start->next->token == PIPE)
	{
		tmp = trim_rout(start, var);
		if (!tmp)
			return (NULL);
	}
	else
	{
		tmp = skip_token(start->next, SPACES, true);
		if (check_token(tmp, start, var))
			return (NULL);
		trim_tword(&start, &tmp);
	}
	start = tmp;
	if (start->prev == NULL)
		var->lexer = start;
	return (start);
}

int	check_syntax_redir(t_var *var)
{
	t_word	*start;

	start = var->lexer;
	while (start)
	{
		if (start->token == REDIR_IN || start->token == REDIR_OUT
			|| start->token == REDIR_APPEND || start->token == HERE_DOC)
		{
			start = check_and_trim(start, var);
			if (!start)
				return (-1);
		}
		start = start->next;
	}
	return (0);
}
