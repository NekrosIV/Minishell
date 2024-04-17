/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 11:53:33 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/17 13:02:53 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_var	*var;
	t_env	*envs;

	(void)av;
	if (ac != 1)
	{
		printf("Error: Too many arguments\n");
		return (1);
	}
	envs = NULL;
	init_env(&envs, env);
	free_list_env(&envs);
	while (1)
	{
		var = malloc(sizeof(t_var));
		if (!var)
		{
			printf("Error: malloc failed\n");
			// free
			return (1);
		}
		init_var(var);
		var->env = envs;
		var->line = readline("minirt> ");
		if (!var->line || !ft_strncmp(var->line, "exit", 4))
		{
			if (var->line)
				printf("exit\n"); // free
			free(var->line);
			free(var);
			exit(0);
		}
		printf("line = %s\n", var->line);
		parsing(var);
		free(var->line);
		free(var);
	}
	return (0);
}
