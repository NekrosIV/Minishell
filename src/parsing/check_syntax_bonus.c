/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 13:10:54 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/07 17:59:01 by kasingh          ###   ########.fr       */
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
			return (free_error(var, E_SYNTAX, ")", -2), -1);
		tmp = tmp->next;
	}
	if (count != 0)
		return (free_error(var, E_SYNTAX, ")", -2), -1);
	return (0);
}

int	is_good_arg_in_parenth(t_word *lexer, t_var *var)
{
	int		count_or_and;
	int		count_parenth;
	char	*err;
	bool	pipe;
	int		count_cmd;
	bool	can_i;
	t_word	*tmp;

	tmp = 0;
	err = NULL;
	can_i = true;
	while (lexer)
	{
		if (lexer->token == PARENTH_OPEN)
		{
			count_or_and = 0;
			count_parenth = 0;
			count_cmd = 0;
			pipe = false;
			tmp = lexer->next;
			while (tmp)
			{
				if (tmp->token == OR || tmp->token == AND)
					count_or_and++;
				if (is_cmd(tmp->token))
					count_cmd++;
				if (tmp->token == PIPE)
					pipe = true;
				if (tmp->token == PARENTH_OPEN)
				{
					count_parenth = 2;
					while (count_parenth > 1)
					{
						tmp = tmp->next;
						if (!err && tmp->token != SPACES)
							err = tmp->word;
						if (tmp->token == PIPE)
							pipe = true;
						if (tmp->token == PARENTH_OPEN)
							count_parenth++;
						if (tmp->token == PARENTH_CLOSE)
							count_parenth--;
						if (count_parenth == 1)
							tmp = tmp->next;
					}
				}
				if (count_parenth == 1 && can_i == true && tmp->token != SPACES)
					(err = tmp->word, can_i = false, printf("err: %s\n", err));
				if (tmp->token == PARENTH_CLOSE)
					break ;
				tmp = tmp->next;
			}
			if (count_parenth == 0 && count_cmd != 0)
				;
			else if (pipe == true && count_parenth != 0)
				return (free_error(var, E_SYNTAX, "|", -2), -1);
			else if (err != NULL && count_or_and <= 0)
				return (free_error(var, E_SYNTAX, err, -2), -1);
			else if (count_or_and <= 0)
				return (free_error(var, E_SYNTAX, "(", -2), -1);
			else if (count_cmd <= 0)
				return (free_error(var, E_SYNTAX, "(", -2), -1);
		}
		lexer = lexer->next;
	}
	return (0);
}

int	check_syntax_parenth(t_word *lexer, t_var *var)
{
	if (check_parenth_close(lexer, var) == -1)
		return (-1);
	if (is_good_arg_in_parenth(lexer, var) == -1)
		return (-1);
	return (0);
}
