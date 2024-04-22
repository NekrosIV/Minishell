/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 11:53:33 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/22 14:33:44 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_var	*var;
	t_env	*envs;

	(void)av;
	if (ac != 1)
		free_error(NULL, E_args, NULL, 0);
	envs = NULL;
	init_env(&envs, env);
	while (1)
	{
		var = malloc(sizeof(t_var));
		if (!var)
		{
			free_list_env(&envs);
			free_error(NULL, E_Malloc, "var", 1);
		}
		init_var(var, &envs);
		// ft_printf("minirt> ");
		var->line = readline("minirt> ");
		// var->line = get_next_line(0);
		if (!var->line || !ft_strncmp(var->line, "exit", 4))
		{
			if (var->line)
				ft_putstr_fd("exit\n", 2);
			free_error(var, NULL, NULL, 0);
		}
		parsing(var);
		if (var->error == false)
			before_exe(var);
		free_var(var);
	}
	return (0);
}
