/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
<<<<<<< HEAD:src/parsing/utils2.c
/*   Created: 2023/03/24 17:52:35 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/24 18:21:42 by jvigny           ###   ########.fr       */
=======
/*   Created: 2023/03/11 01:46:21 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/04 10:36:49 by qthierry         ###   ########.fr       */
>>>>>>> origin/Quentin:src/utils.c
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

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

bool	eq(char *s1, char *s2)
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

<<<<<<< HEAD:src/parsing/utils2.c
bool	has_error_for_meta(char *input, size_t i)
{
	if (input[i] == '&' && input[i + 1] != '&')
		return (0);
	else if (input[i] == '<' || input[i] == '>')
	{
		if (!has_argument_right(input + i))
			return (1);
	}
	else if (input[i] == '|' || input[i] == '&')
	{
		if (!has_argument_left(input, (input + i))
			|| !has_argument_right(input + i))
			return (1);
	}
	return (0);
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
			while (*input != (char)space)
				*dest++ = *input++;
		}
		space = ((space + 1) * (*input++ == ' ' || *(input - 1) == '\t'));
		if (space <= 1)
			*dest++ = *(input - 1) + ((' ' - '\t') * (*(input - 1) == '\t'));
	}
	*dest = 0;
	while ((*dest == ' ' || !*dest) && dest != start)
		*(dest--) = 0;
}

int	syntax_errors(char *input)
{
	size_t	i;

	if (quotes_not_closed(input))
		return (2);
	if (has_parenthesis_not_closed(input))
		return (2);
	remove_useless_parenthesis(&input);
	if (has_error_on_operators_and_parenthesis(input))
		return (2);
	remove_multiple_wspaces(input);
	printf("canon : `%s`\n", input);
	i = 0;
	while (input[i])
	{
		if (input[i] == '\"' || input[i] == '\'')
			i += skip_quotes(input + i);
		if ((is_meta_character(input[i])) && has_error_for_meta(input, i))
			return (2);
		i++;
	}
	return (0);
}
=======
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

char	*ft_strdup(const char *s)
{
	char	*res;
	size_t	i;

	res = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (s[i])
	{
		res[i] = ((char *)s)[i];
		i++;
	}
	res[i] = 0;
	return (res);
}

static size_t	get_size(unsigned int n)
{
	size_t	size;

	if (!n)
		return (1);
	size = 0;
	while (n > 0)
	{
		size++;
		n /= 10;
	}
	return (size);
}

char	*ft_itoa(int n)
{
	unsigned int	ncpy;
	char			*res;
	size_t			size;

	size = (n < 0);
	ncpy = n * (1 | -size);
	size += get_size(ncpy);
	res = malloc(sizeof(char) * (size + 1));
	if (!res)
		return (NULL);
	res[size--] = '\0';
	if (!ncpy)
		res[0] = '0';
	while (ncpy > 0)
	{
		res[size] = (ncpy % 10) + '0';
		ncpy /= 10;
		size--;
	}
	if (n < 0)
		res[0] = '-';
	return (res);
}

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
		((char *)s)[i++] = 0;
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*res;

	if (!nmemb || !size)
		return (malloc(0));
	if ((size_t)-1 / nmemb < size)
		return (NULL);
	res = malloc(nmemb * size);
	if (!res)
		return (res);
	ft_bzero(res, nmemb * size);
	return (res);
}
>>>>>>> origin/Quentin:src/utils.c
