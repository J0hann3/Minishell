/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prefix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:34:00 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/13 16:23:38 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	get_cleaned_name(char *dst, t_char *src, size_t size)
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
		if ((src[i].c == '\'' || src[i].c == '"') && src[i].is_inter == true)
		{
			if (is_in_quote && quote == src[i].c)
				is_in_quote = false;
			else if (is_in_quote)
				dst[j--] = src[i].c;
			else
			{
				is_in_quote = true;
				quote = src[i].c;
			}
			i--;
		}
		else
			dst[j--] = src[i--].c;
	}
	dst[size] = 0;
}

static int	get_prefix_size(const t_char *input, const t_char *start)
{
	int		i;
	char	quote;
	size_t	size;

	i = 0;
	size = 0;
	while (input + i >= start && !is_end_of_single_wildcard(input, i))
	{
		if (input[i].c == '/')
			return (ft_write_error(NULL, "wildcard", "forbidden `/' in wildcard pattern"), -1);
		if ((input[i].c == '\'' || input[i].c == '"') && input[i].is_inter == true)
		{
			quote = input[i].c;
			if (input + i <= start)
				break ;
			i--;
			while (input + i >= start)
			{
				if (input[i].c == quote && input[i].is_inter)
					break ;
				if (input[i].c == '/')
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
	return (prefix);
}