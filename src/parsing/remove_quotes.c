/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 18:03:57 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/10 17:31:13 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		if (string[i] == '\'' || string[i] == '"')
		{
			if (is_in_quote && quote == string[i])
				is_in_quote = false;
			else if (!is_in_quote)
			{
				is_in_quote = true;
				quote = string[i];
			}
		}
		else 
			cpy[j++] = string[i];
		i++;
	}
	cpy[j] = 0;
}