/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 19:48:35 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/24 18:21:39 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

bool	has_parenthesis_not_closed(const char *input)
{
	size_t	depth;
	int		i;
	int		j;

	depth = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '(')
		{
			j = 1;
			while (is_wspace(input[i + j]))
				j++;
			if (input[i + j] == ')')
				return (true);
		}
		if (input[i] == '\"' || input[i] == '\'')
			i += skip_quotes(input + i);
		else if (input[i] == '(')
			depth++;
		else if (input[i] == ')')
			depth--;
		i++;
	}
	return (depth != 0);
}

bool	quotes_not_closed(const char *input)
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

bool	has_argument_left(const char *start_input, char *op_ptr)
{
	char	*tmp;

	tmp = op_ptr;
	while (tmp != start_input)
	{
		tmp--;
		if (*tmp == '(')
			return (false);
		if (tmp != start_input && is_double_meta(tmp - 1))
			return (false);
		if (is_single_meta(tmp))
			return (false);
		else if (!is_wspace(*tmp))
			return (true);
	}
	return (false);
}

bool	has_argument_right(char *op_ptr)
{
	if (*(op_ptr + 1) == *op_ptr)
		op_ptr++;
	op_ptr++;
	while (*op_ptr)
	{
		if (*op_ptr == ')')
			return (false);
		if (is_double_meta(op_ptr) || is_single_meta(op_ptr))
			return (false);
		else if (!is_wspace(*op_ptr))
			return (true);
		op_ptr++;
	}
	return (false);
}
