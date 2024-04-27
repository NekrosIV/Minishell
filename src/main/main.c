/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 11:53:33 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/27 19:10:27 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	t_var	*var;
	t_env	*envs;
	int		exit_status;

	(void)av;
	if (ac != 1)
		free_error(NULL, E_ARGS, NULL, 0);
	envs = NULL;
	init_env(&envs, env);
	exit_status = 0;
	while (1)
	{
		var = init_var(&envs, exit_status);
		var->line = readline("minirt> ");
		(check_exit(var), parsing(var));
		if (var->error == false)
			before_exe(var);
		exit_status = var->status;
		free_var(var);
	}
	return (0);
}
