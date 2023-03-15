/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 00:59:40 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/15 18:48:01 by qthierry         ###   ########.fr       */
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

size_t	skip_quotes(char *input)
{
	char	quote;
	size_t	i;

	quote = *input;
	i = 1;
	while (input[i])
	{
		printf("test : %c\n", input[i]);
		if (input[i] == quote)
			break ;
		i++;
	}
	return (i);
}
