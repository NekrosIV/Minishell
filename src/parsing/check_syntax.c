/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/18 16:46:34 by pscala            #+#    #+#             */
/*   Updated: 2024/04/27 17:14:31 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_syntax(t_var *var)
{
	if (check_syntax_pipe(var->lexer, var) == -1)
		var->error = true;
	else if (check_syntax_redir(var) == -1)
		var->error = true;
}
