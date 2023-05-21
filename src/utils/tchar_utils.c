/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tchar_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 00:46:07 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/21 02:29:38 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

size_t	skip_quotes_tchar(const t_char *input)
{
	char	quote;
	size_t	i;

	quote = input->c;
	i = 1;
	while (input[i].c)
	{
		if (input[i].c == quote && input[i].is_inter == true)
			return (i);
		i++;
	}
	return (0);
}

bool	inter_eq(t_char c1, char c2)
{
	return (c1.is_inter && c1.c == c2);
}

bool	tchar_is_operator(const t_char *c)
{
	return ((c->c == '&' && (c + 1)->c == '&' && (c + 1)->is_inter)
		|| (c->c == '|' && (c + 1)->c == '|' && (c + 1)->is_inter)
		|| c->c == '|');
}
