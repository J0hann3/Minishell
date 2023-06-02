/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   char_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 18:38:36 by qthierry          #+#    #+#             */
/*   Updated: 2023/06/02 18:40:18 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t	ft_strlen(const char *s)
{
	const char	*cpy;

	if (!s)
		return (0);
	cpy = s;
	while (*s)
		s++;
	return ((size_t)(s - cpy));
}

char	*ft_strchr(const char *s, int c)
{
	while (*s)
	{
		if (*s == (char)c)
			return ((char *)s);
		s++;
	}
	if (c == 0)
		return ((char *)s);
	return (NULL);
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*res;
	size_t	i;

	res = malloc(sizeof(char) * (n + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (s[i] && i < n)
	{
		res[i] = ((char *)s)[i];
		i++;
	}
	res[i] = 0;
	return (res);
}
