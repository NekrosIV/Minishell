/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 11:53:33 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/08 14:00:15 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*line;

	(void)av;
	if (ac != 1)
	{
		printf("Error: Too many arguments\n");
		return (1);
	}
	printf("Hello, World!\n");
	return (0);
	while (1)
	{
		line = readline("minirt>");
		printf("line = %s", line);
	}
}
