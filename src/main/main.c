/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 11:53:33 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/05 17:32:40 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		exit_status;

void	sigint_handler(int sig)
{
	(void)sig;
	exit_status = 130;
	rl_on_new_line();
	ft_putendl_fd("", 1);
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_handler_child(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}

void	set_signals(void)
{
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

int	main(int ac, char **av, char **env)
{
	t_var	*var;
	t_env	*envs;

	(void)av;
	if (ac != 1)
		free_error(NULL, E_ARGS, NULL, 0);
	envs = NULL;
	init_env(&envs, env);
	exit_status = 0;
	while (1)
	{
		set_signals();
		var = init_var(&envs, exit_status);
		get_line(var);
		(check_exit(var), parsing(var));
		if (var->error == false)
			before_exe(var);
		free_var(var);
	}
	return (0);
}
