/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   skip_quote_par.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 18:16:22 by qthierry          #+#    #+#             */
/*   Updated: 2023/06/02 18:20:08 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
