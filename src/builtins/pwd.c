/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:49:20 by pscala            #+#    #+#             */
/*   Updated: 2024/06/01 17:01:53 by pscala           ###   ########.fr       */
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
	ft_putendl_fd(pwd, 1);
	return (free(pwd), 0);
}
