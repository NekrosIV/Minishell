/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   before_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 14:57:37 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/27 16:46:04 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	loop_here_doc(char *eof, int fd)
{
	char	*line;

	while (1)
	{
		ft_putstr_fd("here_doc> ", 1);
		line = get_next_line(0);
		if (!line || ft_strncmp(line, eof, ft_strlen(line)) == 0)
		{
			if (!line)
			{
				ft_putstr_fd("\nhere-document delimited ", 2);
				ft_putstr_fd("by end-of-file (wanted `", 2);
				ft_putstr_fd(eof, 2);
				ft_putendl_fd("')", 2);
			}
			free(line);
			break ;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
}

int	here_doc(t_word *tmp, t_var *var)
{
	char		*eof;
	static int	i = 0;
	int			fd;
	char		*file_name;
	char		*nb;

	nb = ft_itoa(i++);
	if (!nb)
		return (free_error(var, E_Malloc, "nb", 1), -1);
	file_name = ft_strjoin("/tmp/here_doc_", nb);
	if (!file_name)
		return (free(nb), free_error(var, E_Malloc, "file_name", 1), -1);
	fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (free(nb), free(file_name), perror("here_doc :"), -1);
	eof = ft_strjoin(tmp->word, "\n");
	if (!eof)
		return (free(nb), free_error(var, E_Malloc, "eof", 1), -1);
	loop_here_doc(eof, fd);
	free(tmp->word);
	tmp->word = file_name;
	return (free(eof), free(nb), close(fd), 1);
}

void	do_here_doc(t_var *var)
{
	t_word	*tmp;

	tmp = var->lexer;
	while (tmp && var->error == false)
	{
		if (tmp->token == HERE_DOC)
		{
			if (here_doc(tmp, var) == -1)
				var->error = true;
		}
		tmp = tmp->next;
	}
}

void	before_exe(t_var *var)
{
	if (node_cmp_token(var->lexer, HERE_DOC) == 1)
		do_here_doc(var);
	if (var->error == false)
		expand(var);
	if (var->error == false)
		exe_cmd(var);
}
