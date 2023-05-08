/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prefix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:34:00 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/08 18:11:19 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_copy(char *dst, char *src, size_t size)
{
	int		i;
	int		j;
	char	quote;
	bool	is_in_quote;

	i = 0;
	j = size - 1;
	quote = 0;
	is_in_quote = false;
	while (j >= 0)
	{
		if (src[i] == '\'' || src[i] == '"')
		{
			if (is_in_quote && quote == src[i])
				is_in_quote = false;
			else if (is_in_quote)
				dst[j--] = src[i];
			else
			{
				is_in_quote = true;
				quote = src[i];
			}
			i--;
		}
		else
			dst[j--] = src[i--];
		printf("j : %d\n", j);
	}
	dst[size] = 0;
}

static int	get_prefix_size(const char *input, const char *start)
{
	int		i;
	int		tmp;
	char	quote;
	size_t	size;

	i = 0;
	size = 0;
	while (input + i >= start && !is_operator(input + i) && !is_wspace(input[i]) && !is_redirection(input[i]) && !is_parenthesis(input[i]) && input[i] != '*')
	{
		if (input[i] == '/')
			return (ft_write_error(NULL, "wildcard", "forbidden `/' in wildcard pattern"), -1);
		if (input[i] == '\'' || input[i] == '"')
		{
			quote = input[i];
			if (input + i <= start)
				break ;
			i--;
			while (input + i >= start && input[i] != quote)
			{
				if (input[i] == '/')
					return (ft_write_error(NULL, "wildcard", "forbidden `/' in wildcard pattern"), -1);
				i--;
				size++;
			}
			if (input + i <= start)
				break ;
			i--;
		}
		else
		{
			size++;
			i--;
		}
	}
	return (size);
}

char	*get_prefix(const char *input, const char *start)
{
	int	pattern_size;
	char	*prefix;

	if (input == start)
		return (ft_strdup(""));
	input--;
	pattern_size = get_prefix_size(input, start);
	if (pattern_size < 0)
		return (NULL);
	prefix = ft_calloc(pattern_size + 1, sizeof(char));
	if (!prefix)
		return (NULL); // error write
	ft_copy(prefix, (char *)input, pattern_size);
	printf("prefix : '%s'\n", prefix);
	return (prefix);
}