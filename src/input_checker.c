/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 19:48:35 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/12 20:24:14 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

int	is_meta_character(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == '&');
}

int	is_wspace(int c)
{
	return (c == ' ' || (c >= 9 && c <= 13));
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

int	has_error_syntax_pipe(const char *input)
{
	char	*pipe_pos;

	pipe_pos = (char *)input;
	while (pipe_pos)
	{
		pipe_pos = ft_strchr(pipe_pos, '|');
		if (!pipe_pos)
			break ;
		if (*(pipe_pos + 1) == '|')
		{
			pipe_pos += 2;
			input = pipe_pos;
			continue ;
		}
		if (!find_argument(input, pipe_pos - input))
			return (1);
		if (!find_argument(pipe_pos + 1, ft_strlen(pipe_pos)))
			return (1);
		pipe_pos += 1;
		input = pipe_pos;
	}
	return (0);
}
