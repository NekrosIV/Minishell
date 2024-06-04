/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   before_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:57:37 by kasingh           #+#    #+#             */
/*   Updated: 2024/06/04 15:01:33 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	loop_here_doc(char *eof, int fd, t_word *tmp, t_var *var)
{
	char	*line;

	line = NULL;
	while (1)
	{
		if (isatty(0))
			ft_putstr_fd("here_doc> ", 1);
		line = get_next_line(0);
		if (g_exit_status == -999)
		{
			free(line);
			break ;
		}
		if (!line || ft_strncmp(line, eof, ft_strlen(line)) == 0)
		{
			if (!line)
				free_error(NULL, E_HEREDOC, tmp->word, -1);
			free(line);
			break ;
		}
		if (tmp->here_doc_expand == true && ft_strchr(line, '$') != NULL)
			line = expand_here_doc_line(line, var);
		write(fd, line, ft_strlen(line));
		free(line);
	}
}

int	here_doc(t_word *tmp, t_var *var, char *file_name)
{
	char	*eof;
	int		fd;

	g_exit_status = 0;
	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (free(file_name), perror("here_doc :"), -1);
	free(file_name);
	eof = ft_strjoin(tmp->word, "\n");
	if (!eof)
		return (free_error(var, E_MALLOC, "eof", 1), -1);
	loop_here_doc(eof, fd, tmp, var);
	return (free(eof), close(fd), g_exit_status);
}

void	fork_here_doc(t_var *var, t_word *tmp, char *file_name)
{
	pid_t	pid;
	int		e_xit;

	pid = fork();
	if (pid == -1)
		return (free_error(var, "fork failed", "do_here_doc", 1));
	if (pid == 0)
	{
		signal(SIGINT, &sigint_handler_here_doc);
		e_xit = here_doc(tmp, var, file_name);
		if (e_xit == -999)
			e_xit = 130;
		var->exit = true;
		free_var(var);
		exit(e_xit);
	}
	free(tmp->word);
	tmp->word = file_name;
	g_exit_status = wait_for_child(pid);
	if (g_exit_status != 0)
	{
		unlink(tmp->word);
		var->error = true;
	}
}

void	do_here_doc(t_var *var)
{
	t_word	*tmp;
	char	*nb;
	char	*file_name;

	tmp = var->lexer;
	signal(SIGINT, &sigint_handler_child);
	while (tmp && var->error == false)
	{
		if (tmp->token == HERE_DOC)
		{
			nb = ft_itoa(var->here_doc_count++);
			if (!nb)
				free_error(var, E_MALLOC, "nb", 1);
			file_name = ft_strjoin("here_doc_", nb);
			if (!file_name)
				free_error(var, E_MALLOC, "file_name", 1);
			free(nb);
			fork_here_doc(var, tmp, file_name);
		}
		tmp = tmp->next;
	}
}

void	join_node2(t_word *head, t_var *var)
{
	t_word	*end;
	char	*str;
	t_word	*start;
	t_word	*temp;

	if (head->next == NULL)
		return ;
	end = head->next;
	start = head->next;
	while (end->token == CMD || end->token == DOUBLE_QUOTE
		|| end->token == SINGLE_QUOTE)
		end = end->next;
	if (start == end)
		return ;
	str = ft_strjoin_tword(head, var, head->token);
	(free(head->word), head->word = str);
	while (start != end)
	{
		temp = start->next;
		(del_tword(&start), start = temp);
	}
}

void	join_node(t_var *var)
{
	t_word	*tmp;

	tmp = var->lexer;
	while (tmp->token != END)
	{
		if (tmp->token == CMD || tmp->token == DOUBLE_QUOTE 
			|| tmp->token == SINGLE_QUOTE || tmp->token == REDIR_IN
			|| tmp->token == REDIR_OUT || tmp->token == REDIR_APPEND)
			join_node2(tmp, var);
		tmp = tmp->next;
	}
}

void	before_exe(t_var *var)
{
	t_word	*tmp;

	tmp = var->lexer;
	tmp = skip_token(tmp, SPACES, true);
	if (tmp->token == END)
		var->error = true;
	if (var->error == false && node_cmp_token(var->lexer, HERE_DOC) == 1)
		do_here_doc(var);
	signal(SIGINT, &sigint_handler_child);
	if (var->error == false)
		expand(var);
	if (var->error == false)
		do_wildcard(var);
	// print_list(var->lexer);
	if (var->error == false)
		join_node(var);
	if (var->error == false)
		exe_cmd(var);
}
