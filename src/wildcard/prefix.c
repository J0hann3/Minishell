/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prefix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:34:00 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/18 16:20:24 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief get the name of a prefix without quotes (reverse)
 * 
 * @param dst 
 * @param src 
 * @param size 
 */
static void	get_cleaned_name(char *dst, t_char *src, size_t size)
{
	int		i;
	int		j;
	char	quote;
	bool	is_in_quote;

	i = 0;
	j = size - 1;
	is_in_quote = false;
	while (j >= 0)
	{
		if ((src[i].c == '\'' || src[i].c == '"') && src[i].is_inter == true)
		{
			if (is_in_quote && quote == src[i--].c)
				is_in_quote = false;
			else if (is_in_quote)
				dst[j--] = src[i--].c;
			else
			{
				is_in_quote = true;
				quote = src[i--].c;
			}
		}
		else
			dst[j--] = src[i--].c;
	}
}

static bool	skip_quote_reverse(const t_char *input,
		const t_char *start, int *i, size_t *size)
{
	char	quote;

	quote = input[*i].c;
	if (input + *i <= start)
		return (true);
	(*i)--;
	while (input + *i >= start)
	{
		if (input[*i].c == quote && input[*i].is_inter)
			break ;
		if (input[*i].c == '/')
			return (ft_write_error(NULL,
					"wildcard", "forbidden `/' in wildcard pattern"), false);
		(*i)--;
		(*size)++;
	}
	(*i)--;
	return (true);
}

static int	get_prefix_size(const t_char *input, const t_char *start)
{
	int		i;
	size_t	size;

	i = 0;
	size = 0;
	while (input + i >= start && !is_end_of_single_wildcard(input, i))
	{
		if (input[i].c == '/')
			return (ft_write_error(NULL, "wildcard",
					"forbidden `/' in wildcard pattern"), -1);
		if ((input[i].c == '\'' || input[i].c == '"')
			&& input[i].is_inter == true)
		{
			if (!skip_quote_reverse(input, start, &i, &size))
				return (-1);
		}
		else
		{
			size++;
			i--;
		}
	}
	return (size);
}

char	*get_prefix(const t_char *input, const t_char *start)
{
	int		prefix_size;
	char	*prefix;

	if (input == start)
		return (ft_strdup(""));
	input--;
	prefix_size = get_prefix_size(input, start);
	if (prefix_size < 0)
		return (NULL);
	prefix = ft_calloc(prefix_size + 1, sizeof(char));
	if (!prefix)
		return (mem_exh("wildcard"), NULL);
	get_cleaned_name(prefix, (t_char *)input, prefix_size);
	prefix[prefix_size] = 0;
	return (prefix);
}
