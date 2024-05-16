/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 11:53:33 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/16 19:09:09 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_exit_status;

void	sigint_handler(int sig)
{
	(void)sig;
	g_exit_status = 130;
	rl_on_new_line();
	print_exit_status(get_terminal_width(), false);
	if (!isatty(0))
		ft_putstr_fd("\n", 1);
	rl_replace_line("", 0);
	rl_reset_line_state();
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

void	sigint_handler_here_doc(int signum)
{
	(void)signum;
	close(0);
	g_exit_status = -999;
}

int	main(int ac, char **av, char **env)
{
	t_var	*var;
	t_env	*envs;
	bool	allias_ls;

	(void)av;
	allias_ls = true;
	if (ac != 1)
		free_error(NULL, E_ARGS, NULL, 0);
	envs = NULL;
	init_env(&envs, env);
	g_exit_status = 0;
	while (1)
	{
		set_signals();
		var = init_var(&envs, allias_ls);
		get_line(var);
		(check_exit(var), parsing(var));
		if (var->error == false)
			before_exe(var);
		allias_ls = var->use_ls_alias;
		free_var(var);
		print_exit_status(get_terminal_width(), true);
	}
	return (0);
}
