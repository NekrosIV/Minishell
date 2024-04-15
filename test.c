/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:59:06 by pscala            #+#    #+#             */
/*   Updated: 2024/04/13 15:28:27 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

void	testi(int *i)
{
	(*i)++;
}

int	main(void)
{
	char *str;
	int i;

	str = "bonjour";
	i = 0;
	printf("pre appel : %d\n", i);
	while (str[i])
	{
		if (str[i] == 'o')
			testi(&i);
		i++;
	}
	printf("post appel : %d\n", i);
	return (0);
}