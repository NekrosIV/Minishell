/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 11:53:33 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/10 15:33:21 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_var	*var;

	(void)av;
	(void)env;
	if (ac != 1)
	{
		printf("Error: Too many arguments\n");
		return (1);
	}
	while (1)
	{
		var = malloc(sizeof(t_var));
		if (!var)
		{
			printf("Error: malloc failed\n");
			return (1);
		}
		init_var(var);
		var->line = readline("minirt> ");
		if (!var->line || !ft_strncmp(var->line, "exit", 4))
		{
			if (var->line)
				printf("exit\n");
			free(var->line);
			exit(0);
		}
		printf("line = %s\n", var->line);
		parsing(var);
		free(var->line);
		free(var);
	}
	return (0);
}
