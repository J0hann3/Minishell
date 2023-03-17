/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 00:59:40 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/17 03:51:50 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

bool	is_meta_character(char c)
{
	return (ft_strchr("<>|&", c) != 0);
}

bool	is_wspace(int c)
{
	return (c == ' ' || c == '\t');
}

bool	is_and_or(const char *input)
{
	return (*input == '&' && *(input + 1) == '&'
			|| *input == '|' && *(input + 1) == '|');
}

// returns the closing quote of the one entered
size_t	skip_quotes(const char *input)
{
	char	quote;
	size_t	i;

	quote = *input;
	i = 1;
	while (input[i])
	{
		if (input[i] == quote)
			return (i);
		i++;
	}
	return (0);
}

// returns the closing parenthesis of the one entered
size_t	skip_parenthesis(const char *input)
{
	size_t	i;

	i = 1;
	while (input[i])
	{
		if (input[i] == ')')
			return (i);
		else if (input[i] == '(')
			i += skip_parenthesis(input + i);
		i++;
	}
	return (0);
}
