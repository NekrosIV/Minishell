/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus_cmd3.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/08 20:31:09 by pscala            #+#    #+#             */
/*   Updated: 2024/06/08 20:31:54 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
