/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 19:48:35 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/17 04:03:17 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

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
		if (!is_meta_character(*tmp) && !is_wspace(*tmp))
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
		if (!is_meta_character(*op_ptr) && !is_wspace(*op_ptr))
			return (1);
		if (is_meta_character(*op_ptr))
			return (0);
		op_ptr++;
	}
	return (0);
}
