/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   others_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 01:46:21 by qthierry          #+#    #+#             */
/*   Updated: 2023/06/02 18:43:55 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Test if two string are equal
 * Returns true if yes, false either
 * 
 * @param s1 
 * @param s2 
 * @return true 
 * @return false 
 */
bool	eq(const char *s1, const char *s2)
{
	size_t	i;

	if (!s1 && !s2)
		return (true);
	if (!s1 || !s2)
		return (false);
	i = 0;
	if (ft_strlen(s1) != ft_strlen(s2))
		return (false);
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (false);
		i++;
	}
	return (true);
}

/**
 * @brief Test if two string are equal on n bytes
 * Returns true if yes or n is 0, false either
 * 
 * @param s1 
 * @param s2 
 * @param n 
 * @return true 
 * @return false 
 */
bool	eqn(const char *s1, const char *s2, size_t n)
{
	size_t	i;

	if (!n || (!s1 && !s2))
		return (true);
	if (!s1 || !s2)
		return (false);
	i = 0;
	while ((s1[i] || s2[i]) && i < n)
	{
		if (s1[i] != s2[i])
			return (false);
		i++;
	}
	return (true);
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

void	*ft_realloc(void *ptr, size_t prev_size, size_t new_size)
{
	void	*new_ptr;

	new_ptr = ft_calloc(new_size, 1);
	if (!new_ptr)
		return (NULL);
	if (prev_size > new_size)
		prev_size = new_size;
	ft_memcpy(new_ptr, ptr, prev_size);
	free(ptr);
	return (new_ptr);
}

void	remove_multiple_wspaces(char *input)
{
	char	*dest;
	char	*start;
	size_t	space;

	space = 1;
	dest = input;
	start = input;
	while (*input)
	{
		if (*input == '\'' || *input == '\"')
		{
			space = *input;
			*dest++ = *input++;
			while (*input && *input != (char)space)
				*dest++ = *input++;
		}
		if (*input == '\0')
			break ;
		space = ((space + 1) * (*input++ == ' ' || *(input - 1) == '\t'));
		if (space <= 1)
			*dest++ = *(input - 1) + ((' ' - '\t') * (*(input - 1) == '\t'));
	}
	*dest = 0;
	while ((*dest == ' ' || *dest == 0) && dest != start)
		*(dest--) = 0;
}
