/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   before_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:57:37 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/06 16:28:40 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	loop_here_doc(char *eof, int fd, t_word *tmp, t_var *var)
{
	char	*line;
	char	*expanded_line;
	char	*tmp_line;

	(expanded_line = NULL, line = NULL, tmp_line = NULL);
	while (1)
	{
		if (isatty(0))
			ft_putstr_fd("here_doc> ", 1);
		line = get_next_line(0);
		if (exit_status == -999)
		{
			free(line);
			break ;
		}
		if (!line || ft_strncmp(line, eof, ft_strlen(line)) == 0)
		{
			if (!line)
			{
				ft_putstr_fd("\nhere-document delimited ", 2);
				ft_putstr_fd("by end-of-file (wanted `", 2);
				ft_putstr_fd(tmp->word, 2);
				ft_putendl_fd("')", 2);
			}
			free(line);
			break ;
		}
		if (tmp->here_doc_expand == true && ft_strchr(line, '$') != NULL)
		{
			tmp_line = ft_strtrim(line, "\n");
			if (!tmp_line)
				(free(line), free_error(var, E_MALLOC, "tmp_line", 1));
			expanded_line = find_in_env(tmp_line, var);
			(free(line), free(tmp_line));
			if (!expanded_line)
				line = ft_strdup("\n");
			else
				line = ft_strjoin(expanded_line, "\n");
			if (!line)
				(free(expanded_line), free_error(var, E_MALLOC, "line", 1));
			free(expanded_line);
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
}

int	here_doc(t_word *tmp, t_var *var, char *file_name)
{
	char	*eof;
	int		fd;

	exit_status = 0;
	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (free(file_name), perror("here_doc :"), -1);
	free(file_name);
	eof = ft_strjoin(tmp->word, "\n");
	if (!eof)
		return (free_error(var, E_MALLOC, "eof", 1), -1);
	loop_here_doc(eof, fd, tmp, var);
	return (free(eof), close(fd), exit_status);
}
void	sigint_handler_here_doc(int signum)
{
	(void)signum;
	close(0);
	exit_status = -999;
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
	exit_status = wait_for_child(pid);
	if (exit_status != 0)
	{
		unlink(tmp->word);
		var->error = true;
	}
}

void	do_here_doc(t_var *var)
{
	t_word	*tmp;
	pid_t	pid;
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

void	before_exe(t_var *var)
{
	if (node_cmp_token(var->lexer, HERE_DOC) == 1)
		do_here_doc(var);
	signal(SIGINT, &sigint_handler_child);
	if (var->error == false)
		expand(var);
	print_list(var->lexer);
	if (var->error == false)
		exe_cmd(var);
}
