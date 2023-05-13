/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 18:03:57 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/13 17:38:49 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*remove_quotes(t_char *string)
{
	bool	is_in_quote;
	char	quote;
	size_t	i;
	size_t	j;
	char	*res;

	i = 0;
	j = 0;
	is_in_quote = false;
	res = ft_calloc(ft_tchar_len(string) + 1, sizeof(char));
	if (!res)
		return (mem_exh("token creation"), NULL);
	while (string[i].c)
	{
		if ((string[i].c == '\'' || string[i].c == '"') && string[i].is_inter == true)
		{
			if (is_in_quote && quote == string[i].c)
				is_in_quote = false;
			else if (!is_in_quote)
			{
				is_in_quote = true;
				quote = string[i].c;
			}
			else
				res[j++] = string[i].c;
		}
		else 
			res[j++] = string[i].c;
		i++;
	}
	res[j] = 0;
	return (res);
}
