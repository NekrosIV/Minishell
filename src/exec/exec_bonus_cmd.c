/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus_cmd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 13:44:12 by kasingh           #+#    #+#             */
/*   Updated: 2024/06/08 13:46:40 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_bonus_cmd(t_word *lexer)
{
	while (lexer->token != PIPE && lexer->token != END)
	{
		if (lexer->token == OR || lexer->token == AND
			|| lexer->token == PARENTH_OPEN)
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
		if ((tmp->token == AND && g_exit_status == 0) || (tmp->token == OR
				&& g_exit_status != 0))
			var->execute_next = true;
		else
			var->execute_next = false;
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
		var->in_parenth = true;
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

int	can_i_run_without_fork(t_var *var, t_word *tmp, t_word *head, int i)
{
	if (var->in_parenth == false && head->prev == NULL && i != 0)
		return (0);
	if (var->execute_next && is_cmd(tmp->token) == 1 && is_builtins(tmp) != 0)
	{
		while (is_cmd(tmp->token) == 1 || tmp->token == SPACES)
			tmp = tmp->next;
		if (tmp->token != PIPE)
			return (1);
	}
	return (0);
}

void	run_without_fork(t_var *var, t_word **head, int c_fd, int pipe_fd[2])
{
	t_word	*tmp;

	tmp = var->lexer;
	var->lexer = *head;
	g_exit_status = do_bultins(var);
	var->lexer = tmp;
	if (var->exit == true)
	{
		if (c_fd >= 0)
			close(c_fd);
		(close(pipe_fd[0]), close(pipe_fd[1]));
		free_error(var, NULL, NULL, g_exit_status);
	}
}

bool	run_in_fork(t_var *var, t_word *tmp, bool flag)
{
	return (var->execute_next && (is_cmd(tmp->token) == 1
			|| tmp->token == PARENTH_OPEN) && flag == true);
}

int	do_bonus_cmd(int c_fd, int pipe_fd[2], int i, t_var *var)
{
	pid_t	pid;
	t_word	*tmp;
	t_word	*h;
	bool	flag;

	tmp = var->lexer;
	h = tmp;
	flag = true;
	while (tmp != NULL && tmp->token != END && tmp->token != PIPE)
	{
		if (can_i_run_without_fork(var, tmp, h, i) && flag == true)
			(run_without_fork(var, &h, c_fd, pipe_fd), flag = false);
		else if (run_in_fork(var, tmp, flag))
		{
			pid = fork();
			if (pid == -1)
				return (close_all_fd(pipe_fd, c_fd, i, true), -1);
			if (pid == 0)
				(new_lst(&h, &var->lexer, tmp), child(c_fd, pipe_fd, i, var));
			(need_to_wait(tmp, pid), flag = false);
		}
		flag = update_execution_state(&h, tmp, var, flag);
		tmp = update_tmp(tmp, var);
	}
	return (0);
}
