/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 01:46:21 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/15 03:18:35 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

size_t	ft_strlen(const char *s)
{
	const char	*cpy;

	cpy = s;
	while (*s)
		s++;
	return ((size_t)(s - cpy));
}

int	equals(char	*s1, char *s2)
{
	size_t	i;

	if (!s1 || !s2)
		return (-1);
	i = 0;
	if (ft_strlen(s1) != ft_strlen(s2))
		return (0);
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (0);
		i++;
	}
	return (1);
}

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	size_t	i;
	size_t	size;

	size = sizeof(size_t);
	i = 0;
	if (n > size)
	{
		while (i < n - size)
		{
			*(size_t *)(dest + i) = *(size_t *)(src + i);
			i += size;
		}
	}
	while (i != n)
	{
		*(unsigned char *)(dest + i) = *(unsigned char *)(src + i);
		i++;
	}
	return (dest);
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

void	*ft_realloc(void *ptr, size_t prev_size, size_t new_size)
{
	void	*new_ptr;

	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	if (prev_size > new_size)
		prev_size = new_size;
	ft_memcpy(new_ptr, ptr, prev_size);
	free(ptr);
	return (new_ptr);
}
