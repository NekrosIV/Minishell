/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:10:54 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/10 14:49:30 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_before_and_affter(t_word *tmp, bool dir)
{
	if (dir == true)
	{
		tmp = tmp->next;
		while (tmp)
		{
			if (tmp->token == SPACES || tmp->token == END)
				tmp = tmp->next;
			else
				break ;
		}
	}
	else
	{
		tmp = tmp->prev;
		while (tmp)
		{
			if (tmp->token == SPACES || tmp->token == END)
				tmp = tmp->prev;
			else
				break ;
		}
	}
	if (!tmp || tmp->token == PIPE || tmp->token == OR || tmp->token == AND
		|| (tmp->token == PARENTH_CLOSE && dir == true)
		|| (tmp->token == PARENTH_OPEN && dir == false))
		return (-1);
	return (0);
}

int	check_syntax_or_and(t_word *lexer, t_var *var)
{
	t_word	*tmp;

	tmp = 0;
	while (lexer)
	{
		if (lexer->token == OR || lexer->token == AND)
		{
			tmp = lexer;
			if (check_before_and_affter(tmp, true) == -1
				|| check_before_and_affter(tmp, false) == -1)
				return (free_error(var, E_SYNTAX, lexer->word, -2), -1);
		}
		lexer = lexer->next;
	}
	return (0);
}

int	is_cmd(int token)
{
	if (token != SPACES && token != PARENTH_OPEN && token != PARENTH_CLOSE
		&& token != END && token != OR && token != AND && token != PIPE)
		return (1);
	return (0);
}

int	check_parenth_close(t_word *tmp, t_var *var)
{
	int	count;

	count = 0;
	while (tmp)
	{
		if (tmp->token == PARENTH_OPEN)
			count++;
		else if (tmp->token == PARENTH_CLOSE)
			count--;
		if (count < 0)
			break ;
		tmp = tmp->next;
	}
	if (count != 0)
	{
		free_error(NULL, E_EOF, ")", -2);
		return (free_error(NULL, E_S_EOF, NULL, -2), -1);
	}
	return (0);
}

// int	is_good_arg_in_parenth(t_word *lexer, t_var *var)
// {
// 	int		count_or_and;
// 	int		count_parenth;
// 	char	*err;
// 	bool	pipe;
// 	int		count_cmd;
// 	bool	can_i;
// 	t_word	*tmp;

// 	tmp = 0;
// 	err = NULL;
// 	can_i = true;
// 	while (lexer)
// 	{
// 		if (lexer->token == PARENTH_OPEN)
// 		{
// 			count_or_and = 0;
// 			count_parenth = 0;
// 			count_cmd = 0;
// 			pipe = false;
// 			tmp = lexer->next;
// 			while (tmp)
// 			{
// 				if (tmp->token == OR || tmp->token == AND)
// 					count_or_and++;
// 				if (is_cmd(tmp->token))
// 					count_cmd++;
// 				if (tmp->token == PIPE)
// 					pipe = true;
// 				if (tmp->token == PARENTH_OPEN)
// 				{
// 					count_parenth = 2;
// 					while (count_parenth > 1)
// 					{
// 						tmp = tmp->next;
// 						if (!err && tmp->token != SPACES)
// 							err = tmp->word;
// 						if (tmp->token == PIPE)
// 							pipe = true;
// 						if (tmp->token == PARENTH_OPEN)
// 							count_parenth++;
// 						if (tmp->token == PARENTH_CLOSE)
// 							count_parenth--;
// 						if (count_parenth == 1)
// 							tmp = tmp->next;
// 					}
// 				}
// 				if (count_parenth == 1 && can_i == true && tmp->token != SPACES)
// 					(err = tmp->word, can_i = false, printf("err: %s\n", err));
// 				if (tmp->token == PARENTH_CLOSE)
// 					break ;
// 				tmp = tmp->next;
// 			}
// 			if (count_parenth == 0 && count_cmd != 0)
// 				;
// 			else if (pipe == true && count_parenth != 0)
// 				return (free_error(var, E_SYNTAX, "|", -2), -1);
// 			else if (err != NULL && count_or_and <= 0)
// 				return (free_error(var, E_SYNTAX, err, -2), -1);
// 			else if (count_or_and <= 0)
// 				return (free_error(var, E_SYNTAX, "(", -2), -1);
// 			else if (count_cmd <= 0)
// 				return (free_error(var, E_SYNTAX, "(", -2), -1);
// 		}
// 		lexer = lexer->next;
// 	}
// 	return (0);
// }

t_word	*end_of_parenth(t_word *lexer)
{
	int		count;
	t_word	*tmp;

	if (lexer->token == PARENTH_CLOSE)
		return (lexer);
	count = 1;
	tmp = lexer->next;
	while (tmp)
	{
		if (tmp->token == PARENTH_OPEN)
			count++;
		if (tmp->token == PARENTH_CLOSE)
			count--;
		if (count == 0)
			break ;
		tmp = tmp->next;
	}
	return (tmp);
}

int	check_out_pareth(t_word *lexer, t_var *var)
{
	t_word	*tmp;
	bool	parenthesis;

	parenthesis = false;
	tmp = skip_token(lexer->prev, SPACES, false);
	if (tmp != NULL && tmp->token != PIPE && tmp->token != OR
		&& tmp->token != AND && tmp->token != PARENTH_OPEN)
		return (-1);
	tmp = end_of_parenth(lexer);
	tmp = skip_token(tmp->next, SPACES, true);
	if (tmp->token != END && tmp->token != PIPE && tmp->token != OR
		&& tmp->token != AND && tmp->token != PARENTH_CLOSE)
		return (-1);
	return (0);
}
int	check_in_parenth2(t_word *tmp, bool cmd, bool parenthsis)
{
	tmp = tmp->prev;
	while (tmp)
	{
		if (tmp->token == SPACES || is_cmd(tmp->token))
			tmp = tmp->prev;
		else
			break ;
	}
	if (cmd == true && tmp->token == PARENTH_OPEN)
		return (0);
	else if (tmp->token == PARENTH_CLOSE && parenthsis == true)
		return (-1);
	else if (tmp != NULL && tmp->token != OR && tmp->token != AND
		&& tmp->token != PARENTH_CLOSE)
		return (-1);
	return (0);
}

int	check_in_parenth(t_word *lexer)
{
	t_word	*tmp;
	bool	parenthesis;
	bool	cmd;

	cmd = false;
	tmp = lexer->next;
	parenthesis = false;
	while (tmp)
	{
		if (tmp->token == SPACES || is_cmd(tmp->token))
		{
			if (is_cmd(tmp->token))
				cmd = true;
			tmp = tmp->next;
		}
		else
			break ;
	}
	if (cmd == true && tmp->token == PARENTH_CLOSE)
		cmd = true;
	else if (tmp->token == PARENTH_OPEN)
		parenthesis = true;
	else if (tmp != NULL && tmp->token != OR && tmp->token != AND
		&& tmp->token != PARENTH_OPEN)
		return (-1);
	return (check_in_parenth2(end_of_parenth(tmp), cmd, parenthesis));
}

int	check_syntax_parenth(t_word *lexer, t_var *var)
{
	if (check_parenth_close(lexer, var) == -1)
		return (-1);
	while (lexer)
	{
		if (lexer->token == PARENTH_OPEN)
		{
			if (check_out_pareth(lexer, var) == -1 || check_in_parenth(lexer) ==
				-1)
				return (free_error(var, E_SYNTAX, "(", -2), -1);
		}
		lexer = lexer->next;
	}
	return (1);
}
