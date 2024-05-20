/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 13:44:12 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/20 13:27:43 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_bonus_cmd(t_word *lexer)
{
	while (lexer->token != PIPE && lexer->token != END)
	{
		if (lexer->token == OR || lexer->token == AND)
			return (1);
		lexer = lexer->next;
	}
	return (0);
}

void	new_lst(t_word **start, t_word **lexer, t_word *curent)
{
	t_word	*tmp;
	t_word	*end;

	tmp = NULL;
	end = NULL;
	if (curent->token == PARENTH_OPEN)
		end = end_of_parenth(curent);
	if ((*start)->prev != NULL)
		tmp = *lexer;
	*lexer = *start;
	if (end == NULL)
		end = *start;
	while (end->token != PIPE && end->token != END && end->token != OR
		&& end->token != AND)
		end = end->next;
	if ((*lexer)->prev != NULL)
		(*lexer)->prev->next = end->next;
	else
		tmp = end->next;
	(*lexer)->prev = NULL;
	end->next = NULL;
	if (end->token != PIPE)
		end->token = END;
	if (tmp != NULL)
		free_list_lexer(&tmp);
}

bool	update_execution_state(t_word **start, t_word *tmp, t_var *var,
		bool flag)
{
	if (tmp->token == AND || tmp->token == OR)
	{
		var->execute_next = (tmp->token == AND && g_exit_status == 0)
			|| (tmp->token == OR && g_exit_status != 0);
		*start = tmp->next;
		return (true);
	}
	return (flag);
}

t_word	*update_tmp(t_word *tmp, t_var *var)
{
	(void)var;
	if (tmp->token == PARENTH_OPEN)
		tmp = end_of_parenth(tmp);
	else
		tmp = tmp->next;
	return (tmp);
}

t_word	*find_token(t_word *lexer, int token)
{
	while (lexer && lexer->token != token)
		lexer = lexer->next;
	return (lexer);
}

void	do_cmd_in_parenth(int c_fd, int pipe_fd[2], int i, t_var *var)
{
	t_word	*tmp;

	if (node_cmp_token(var->lexer, PARENTH_OPEN))
	{
		tmp = end_of_parenth(find_token(var->lexer, PARENTH_OPEN));
		tmp->next->token = END;
		tmp = var->lexer;
		var->lexer = find_token(var->lexer, PARENTH_OPEN)->next;
		var->lexer->prev->next = NULL;
		var->lexer->prev = NULL;
		free_list_lexer(&tmp);
		do_bonus_cmd(c_fd, pipe_fd, i, var);
		close_all_fd(pipe_fd, c_fd, i, true);
		var->exit = true;
		free_var(var);
		exit(g_exit_status);
	}
}

void	need_to_wait(t_word *tmp, pid_t pid)
{
	if (tmp->token == PARENTH_OPEN)
	{
		tmp = end_of_parenth(tmp);
		if (tmp->next != NULL)
			tmp = skip_token(tmp->next, SPACES, true);
		if (tmp->token == PIPE || (tmp->token == END && ft_strncmp(tmp->word,
					"newline", 7) != 0))
			return ;
	}
	g_exit_status = wait_for_child(pid);
}

int	do_bonus_cmd(int c_fd, int pipe_fd[2], int i, t_var *var)
{
	pid_t	pid;
	t_word	*tmp;
	t_word	*head;
	bool	flag;

	tmp = var->lexer;
	head = tmp;
	flag = true;
	while (tmp != NULL && tmp->token != END && tmp->token != PIPE)
	{
		if (var->execute_next && (tmp->token == CMD
				|| tmp->token == PARENTH_OPEN) && flag == true)
		{
			pid = fork();
			if (pid == -1)
				return (close_all_fd(pipe_fd, c_fd, i, true), -1);
			if (pid == 0)
				(new_lst(&head, &var->lexer, tmp), child(c_fd, pipe_fd, i,
						var));
			(need_to_wait(tmp, pid), flag = false);
		}
		flag = update_execution_state(&head, tmp, var, flag);
		tmp = update_tmp(tmp, var);
	}
	return (0);
}
