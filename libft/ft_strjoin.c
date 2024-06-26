/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kasingh <kasingh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 12:47:13 by kasingh           #+#    #+#             */
/*   Updated: 2024/04/21 16:28:16 by kasingh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>

/* ft_strjoin: Concatène deux chaînes */
char	*ft_strjoin(char const *s1, char const *s2)
{
	int		i;
	int		j;
	char	*result;

	i = -1;
	j = 0;
	if (!s1 || !s2)
		return (0);
	result = malloc((ft_strlen((char *)s1) + ft_strlen((char *)s2) + 1)
			* sizeof(char));
	if (result == 0)
		return (0);
	while (s1[++i])
	{
		result[j] = s1[i];
		j++;
	}
	i = -1;
	while (s2[++i])
	{
		result[j] = s2[i];
		j++;
	}
	result[j] = '\0';
	return (result);
}
