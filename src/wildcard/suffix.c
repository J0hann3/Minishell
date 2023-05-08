/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   suffix.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 17:34:02 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/08 21:52:00 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	ft_copy(char *dst, char *src, size_t size)
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

static size_t	get_file_size(const char *input, bool *is_end)
{
	size_t	i;
	int		tmp;
	size_t	size;

	i = 0;
	size = 0;
	while (input[i] && !is_operator(input + i) && !is_wspace(input[i]) && !is_redirection(input[i]) && !is_parenthesis(input[i]) && input[i] != '*')
	{
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
	size_t	pattern_size;
	char	*suffix;

	*is_end = false;
	input++;
	pattern_size = get_file_size(input, is_end);
	suffix = ft_calloc(pattern_size + 1, sizeof(char));
	if (!suffix)
		return (NULL); // error write
	ft_copy(suffix, (char *)input, pattern_size);
	printf("suffix : '%s'\n", suffix);
	return (suffix);
}
