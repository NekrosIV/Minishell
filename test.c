/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pscala <pscala@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 16:59:06 by pscala            #+#    #+#             */
/*   Updated: 2024/04/12 17:23:15 by pscala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./include/minishell.h"

char	*ft_strndup(char *line, int i, int start)
{
	char	*str;
	int		j;
	int		size;

	size = i - start;
	j = 0;
	str = malloc(sizeof(char) * size + 1);
	if (!str)
		return (NULL);
	while (j < size)
	{
		str[j] = line[start];
		j++;
		start++;
	}
	str[j] = '\0';
	return (str);
}

int	main(int ac, char **av)

{
	char *str;
	if (ac == 2)
	{
		str = ft_strndup(av[1], 7, 3);
		printf("%s\n", str);
		free(str);
	}
	return (0);
}