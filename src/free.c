/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:34:18 by pscala            #+#    #+#             */
/*   Updated: 2024/04/17 15:07:10 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_list_lexer(t_word **lexer)
{
	t_word	*temp;
	t_word	*current;

	if (*lexer)
	{
		current = *lexer;
		while (current)
		{
			temp = current->next;
			free(current->word);
			free(current);
			current = temp;
		}
		*lexer = NULL;
	}
}

void	free_list_env(t_env **env)
{
	t_env	*temp;
	t_env	*current;

	if (*env)
	{
		current = *env;
		while (current)
		{
			temp = current->next;
			free(current->line);
			free(current);
			current = temp;
		}
		*env = NULL;
	}
}

void	free_var(t_var *var)
{
	if (var->exit == true)
		free_list_env(&var->env);
	free(var->line);
	free_list_lexer(&var->lexer);
	free(var);
}

void	free_error(t_var *var, char *error, char *fautif, int ff)
{
	if (ff >= 0 && var)
		var->exit = true;
	if (var)
		free_var(var);
	if (error)
		ft_putstr_fd(error, 2);
	if (fautif)
		ft_putendl_fd(fautif, 2);
	if (ff >= 0)
		exit(ff);
}