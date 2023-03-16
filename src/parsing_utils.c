/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 00:59:40 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/16 18:20:21 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

bool	is_meta_character(char c)
{
	return (ft_strchr("<>|&()", c) != 0);
}

bool	is_wspace(int c)
{
	return (c == ' ' || c == '\t');
}

size_t	skip_quotes(const char *input)
{
	char	quote;
	size_t	i;

	quote = *input;
	i = 1;
	while (input[i])
	{
		if (input[i] == quote)
			break ;
		i++;
	}
	return (i);
}
