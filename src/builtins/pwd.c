/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:49:20 by pscala            #+#    #+#             */
/*   Updated: 2024/06/03 18:41:40 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	pwd(char **cmd, t_var *var)
{
	char	*pwd;

	(void)cmd;
	(void)var;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		free_error(NULL, "pwd: " E_DIR_ERROR E_GETCWD_ERROR, strerror(errno),
			-1);
		return (1);
	}
	if (write(1, pwd, ft_strlen(pwd)) == -1)
		return (perror("pwd():"), 1);
	if (write(1, "\n", 1) == -1)
		return (perror("pwd():"), 1);
	return (free(pwd), 0);
}
