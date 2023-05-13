/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   suffix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:34:02 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/13 16:33:08 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	get_cleaned_name(char *dst, t_char *src, size_t size)
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
		if ((src[i].c == '\'' || src[i].c == '"') && src[i].is_inter == true)
		{
			if (is_in_quote && quote == src[i].c)
				is_in_quote = false;
			else if (is_in_quote)
				dst[j++] = src[i].c;
			else
			{
				is_in_quote = true;
				quote = src[i].c;
			}
			i++;
		}
		else
			dst[j++] = src[i++].c;
	}
	dst[j] = 0;
}

static ssize_t	get_pattern_size(const t_char *input, bool *is_end)
{
	size_t	i;
	int		tmp;
	size_t	size;

	i = 0;
	size = 0;
	while (input[i].c && !is_end_of_single_wildcard(input, i))
	{
		if (input[i].c == '/' && input[i + 1].c)
			return (ft_write_error(NULL, "wildcard", "forbidden `/' in wildcard pattern"), -1);
		if ((input[i].c == '\'' || input[i].c == '"') && input[i].is_inter == true)
		{
			tmp = skip_quotes_tchar(input + i) + 1;
			i += tmp;
			size += tmp - 2;
		}
		else
		{
			size++;
			i++;
		}
	}
	if (input[i].c != '*')
		*is_end = true;
	return (size);
}

char	*get_suffix(const t_char *input, bool *is_end)
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
		return (mem_exh("wildcard"), NULL);
	get_cleaned_name(suffix, (t_char *)input, pattern_size);
	return (suffix);
}
