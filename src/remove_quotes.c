/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 18:03:57 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/04 18:22:47 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

void	remove_quotes(char *string)
{
	bool	is_in_quote;
	char	quote;
	size_t	i;
	size_t	j;
	char	*cpy;

	cpy = string;
	i = 0;
	j = 0;
	is_in_quote = false;
	while (string[i])
	{
		if ((string[i] == '\"' || string[i] == '\'') && !is_in_quote)
		{
			is_in_quote = true;
			quote = string[i];
		}
		else if (is_in_quote && string[i] == quote)
			is_in_quote = false;
		else
			cpy[j++] = string[i];
		i++;
	}
	cpy[j] = 0;
}