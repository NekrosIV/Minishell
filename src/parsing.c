/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/09 17:19:10 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/09 18:21:06 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	parsing(t_var *var)
{
	int i;

	i = 0;

	while (var->line[i])
	{
		if (var->line[i] == '|')
			printf("PIPE\n");
		else if (var->line[i] == '<')
			printf("REDIR_IN\n");
		else if (var->line[i] == '>')
			printf("REDIR_OUT\n");
		else if (var->line[i] == ' ')
			printf("ARG\n");
		else
			printf("CMD\n");
		i++;
	}
}