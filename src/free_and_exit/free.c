/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 13:34:18 by pscala            #+#    #+#             */
/*   Updated: 2024/05/10 15:56:17 by kasingh          ###   ########.fr       */
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
	free_list_lexer(&var->lexer);
	if (var->tmp != NULL)
		free_list_lexer(&var->tmp);
	free(var->line);
	free(var);
}

void	free_error(t_var *var, char *error, char *fautif, int ff)
{
	if (ff >= 0 && var)
		var->exit = true;
	if (var && ff < 0)
		exit_status = 1 * -ff;
	if (var && ff >= 0)
		free_var(var);
	if (error)
		ft_putstr_fd(error, 2);
	if (fautif)
	{
		ft_putstr_fd(RED, 2);
		ft_putstr_fd("`", 2);
		ft_putstr_fd(fautif, 2);
		ft_putstr_fd("\'\n", 2);
		ft_putstr_fd(RESET, 2);
	}
	if (ff >= 0)
		exit(ff);
}

void	free_split(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}
void	unlink_here_doc(t_var *var)
{
	char	*nb;
	char	*file_name;
	int		i;

	i = 0;
	nb = NULL;
	file_name = NULL;
	while (i <= var->here_doc_count)
	{
		nb = ft_itoa(i++);
		if (!nb)
			free_error(var, E_MALLOC, "nb", 1);
		file_name = ft_strjoin("here_doc_", nb);
		if (!file_name)
			free_error(var, E_MALLOC, "file_name", 1);
		unlink(file_name);
		free(nb);
		free(file_name);
	}
}
