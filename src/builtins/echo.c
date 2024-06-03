/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/01 15:49:09 by pscala            #+#    #+#             */
/*   Updated: 2024/06/03 18:13:55 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_a_good_option(char *str)
{
	size_t	i;

	i = 0;
	if (str[i] != '-')
		return (false);
	i++;
	while (str[i])
	{
		if (str[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

size_t	ft_split_len(char **cmd)
{
	size_t	i;
	size_t	length;

	i = 0;
	length = 0;
	while (cmd[i])
	{
		length += ft_strlen(cmd[i]);
		if (cmd[i + 1])
			length++;
		i++;
	}
	return (length);
}

char	*join_split(char **cmd, bool end_line)
{
	char	*str;
	size_t	i;
	size_t	j;
	size_t	k;

	str = malloc(sizeof(char) * (ft_split_len(cmd) + (end_line == true) + 1));
	if (!str)
		return (NULL);
	i = 0;
	k = 0;
	while (cmd[i])
	{
		j = 0;
		while (cmd[i][j])
			str[k++] = cmd[i][j++];
		if (cmd[i + 1])
			str[k++] = ' ';
		i++;
	}
	if (end_line)
		str[k++] = '\n';
	str[k] = '\0';
	return (str);
}

int	echo(char **cmd, t_var *var)
{
	bool	end_line;
	size_t	i;
	char	*result;

	if (!cmd[1])
		return (write(1, "\n", 1), 0);
	i = 1;
	end_line = true;
	while (cmd[i])
	{
		if (is_a_good_option(cmd[i]))
			end_line = false;
		else
			break ;
		i++;
	}
	result = join_split(cmd + i, end_line);
	if (!result)
		(free_split(cmd), free_error(var, E_MALLOC, "echo():str", 1));
	if (write(1, result, ft_strlen(result)) == -1)
		return (perror("echo():"), free_split(cmd), 1);
	return (free(result), 0);
}
