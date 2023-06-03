/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_arguments_l_r.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 20:00:27 by qthierry          #+#    #+#             */
/*   Updated: 2023/06/02 20:00:45 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	has_argument_left(const char *start_input, char *op_ptr)
{
	char	*tmp;

	tmp = op_ptr;
	while (tmp != start_input)
	{
		tmp--;
		if (*tmp == '(')
			return (ft_write_error(NULL, NULL, get_error_token(op_ptr)), false);
		if (*tmp == '|' || (tmp != start_input && is_and_or(tmp - 1)))
			return (ft_write_error(NULL, NULL, get_error_token(op_ptr)), false);
		else if (!is_wspace(*tmp))
			return (true);
	}
	ft_write_error(NULL, NULL, get_error_token(tmp));
	return (false);
}

/**
 * @brief Get the right argument of the operator.
 * Write error if the argument is not valid.
 * 
 * @param op_ptr 
 * @return true 
 * @return false 
 */
bool	has_argument_right(char *op_ptr)
{
	char	*op_cpy;

	op_cpy = op_ptr;
	if (*(op_ptr + 1) == *op_ptr)
		op_ptr++;
	op_ptr++;
	while (*op_ptr)
	{
		if (*op_ptr == ')')
			return (ft_write_error(NULL, NULL, get_error_token(op_cpy)), false);
		if (is_operator(op_ptr))
			return (ft_write_error(NULL, NULL, get_error_token(op_cpy)), false);
		else if (!is_wspace(*op_ptr))
			return (true);
		op_ptr++;
	}
	ft_write_error(NULL, NULL, get_error_token(op_cpy));
	return (false);
}
