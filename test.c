/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:59:06 by pscala            #+#    #+#             */
/*   Updated: 2024/04/23 16:00:26 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	main(void)
{
	char *args[] = {"cat", NULL};   // Commande à exécuter
	execve("/bin/cat", args, NULL); // Exécute la commande "cat"
	perror("execve");               // En cas d'erreur
	return (EXIT_FAILURE);
}
