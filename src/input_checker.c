/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 19:48:35 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/13 23:24:35 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	is_meta_character(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&');
}

int	is_wspace(int c)
{
	return (c == ' ' || c == '\t');
}

char	*find_non_meta(const char *s, size_t size)
{
	size_t	i;

	i = 0;
	while (s[i] && i < size)
	{
		if (!is_meta_character(s[i]))
			return ((char *)s);
		i++;
	}
	return (NULL);
}

char	*find_argument(const char *input, size_t size)
{
	size_t	i;

	i = 0;
	while (input[i] && i < size)
	{
		if ((!is_meta_character(input[i]) && !is_wspace(input[i]))
			|| input[i] == '&')
			return ((char *)input);
		i++;
	}
	return (NULL);
}

int	quotes_not_closed(const char *input)
{
	int	in_single;
	int	in_double;

	in_single = -1;
	in_double = -1;
	while (*input)
	{
		if (*input == '\"' && in_single == -1)
			in_double *= -1;
		if (*input == '\'' && in_double == -1)
			in_single *= -1;
		input++;
	}
	return (in_double == 1 || in_single == 1);
}

int	has_argument_left(const char *start_input, char *op_ptr)
{
	char	*tmp;

	tmp = op_ptr;
	while (tmp != start_input)
	{
		tmp--;
		if (!is_meta_character(*tmp) && !is_wspace(*tmp))
			return (1);
	}
	return (0);
}

int	has_argument_right(char *op_ptr)
{
	if (*(op_ptr + 1) == *op_ptr)
		op_ptr++;
	op_ptr++;
	while (*op_ptr)
	{
		if (!is_meta_character(*op_ptr) && !is_wspace(*op_ptr))
			return (1);
		if (is_meta_character(*op_ptr))
			return (0);
		op_ptr++;
	}
	return (0);
}
