/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 18:22:45 by qthierry          #+#    #+#             */
/*   Updated: 2023/06/02 20:17:39 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	is_expandable(const char *input)
{
	return (is_alpha(*input) || *input == '_' || *input == '?');
}

size_t	get_size_of_var(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (is_alpha(str[i]) || str[i] == '_' || is_digit(str[i]))
			++i;
		else
			return (i);
	}
	return (i);
}
