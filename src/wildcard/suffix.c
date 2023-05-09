/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   suffix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:34:02 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/10 00:03:23 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	get_cleaned_name(char *dst, char *src, size_t size)
{
	size_t	i;
	size_t	j;
	char	quote;
	bool	is_in_quote;

	i = 0;
	j = 0;
	quote = 0;
	is_in_quote = false;
	while (j < size)
	{
		if (src[i] == '\'' || src[i] == '"')
		{
			if (is_in_quote && quote == src[i])
				is_in_quote = false;
			else if (is_in_quote)
				dst[j++] = src[i];
			else
			{
				is_in_quote = true;
				quote = src[i];
			}
			i++;
		}
		else
			dst[j++] = src[i++];
	}
	dst[j] = 0;
}

static ssize_t	get_pattern_size(const char *input, bool *is_end)
{
	size_t	i;
	int		tmp;
	size_t	size;

	i = 0;
	size = 0;
	while (input[i] && !is_end_of_single_wildcard(input, i))
	{
		if (input[i] == '/')
			return (ft_write_error(NULL, "wildcard", "forbidden `/' in wildcard pattern"), -1);
		if (input[i] == '\'' || input[i] == '"')
		{
			tmp = skip_quotes(input + i) + 1;
			i += tmp;
			size += tmp - 2;
		}
		else
		{
			size++;
			i++;
		}
	}
	if (input[i] != '*')
		*is_end = true;
	return (size);
}

char	*get_suffix(const char *input, bool *is_end)
{
	ssize_t	pattern_size;
	char	*suffix;

	*is_end = false;
	input++;
	pattern_size = get_pattern_size(input, is_end);
	if (pattern_size < 0)
		return (NULL);
	suffix = ft_calloc(pattern_size + 1, sizeof(char));
	if (!suffix)
		return (NULL); // error write
	get_cleaned_name(suffix, (char *)input, pattern_size);
	return (suffix);
}
