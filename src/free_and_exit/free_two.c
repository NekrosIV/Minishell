/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_two.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/11 13:53:00 by kasingh           #+#    #+#             */
/*   Updated: 2024/05/11 13:53:42 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
