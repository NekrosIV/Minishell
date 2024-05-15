/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/27 16:25:15 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/15 16:24:21 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_exit(t_var *var)
{
	if (!var->line || !ft_strncmp(var->line, "exit", 4))
	{
		if (var->line)
			ft_putstr_fd("exit\n", 1);
		free_error(var, NULL, NULL, g_exit_status);
	}
	add_history(var->line);
}

int	get_terminal_width(void)
{
	struct winsize	w;

	if (!isatty(0))
		return (-1);
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
	{
		perror("ioctl");
		return (-1);
	}
	return (w.ws_col);
}

void	print_exit_status(int width, bool is_error)
{
	int		move_right;
	char	move_right_str[20];
	char	*itoa_str;

	if (g_exit_status != 0 && isatty(0))
	{
		move_right = strlen("exit: ");
		move_right += ft_count_digit(g_exit_status);
		if (is_error)
			ft_putstr_fd("\033[F", 2);
		move_right = width - move_right;
		if (move_right > 0)
		{
			itoa_str = ft_itoa(move_right);
			if (itoa_str)
			{
				strcpy(move_right_str, "\033[");
				strcat(move_right_str, itoa_str);
				strcat(move_right_str, "C");
				free(itoa_str), ft_putstr_fd(move_right_str, 2);
			}
		}
		ft_putstr_fd(GREY "exit:" BOLD RED, 2);
		ft_putnbr_fd(g_exit_status, 2), ft_putstr_fd("\n" RESET, 2);
	}
}
