/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 18:03:57 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/21 03:14:51 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// calloc so no \0 at end, norminette issue
static void	copy_string_without_quotes(t_char *string,
	char *res, bool is_in_quote)
{
	size_t	i;
	size_t	j;
	char	quote;

	i = 0;
	j = 0;
	while (string[i].c)
	{
		if ((string[i].c == '\'' || string[i].c == '"')
			&& string[i].is_inter == true)
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
}

char	*remove_quotes(t_char *string)
{
	bool	is_in_quote;
	char	*res;

	is_in_quote = false;
	res = ft_calloc(ft_tchar_len(string) + 1, sizeof(char));
	if (!res)
		return (mem_exh("token creation"), NULL);
	copy_string_without_quotes(string, res, is_in_quote);
	return (res);
}
