/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_file_name.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 02:57:46 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/21 02:59:17 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// in this case, dst is calloc so has a \0 at end, norm issue
static void	ft_copy(char *dst, t_char *src, size_t size)
{
	size_t	i;
	size_t	j;
	char	quote;
	bool	is_in_quote;

	i = 0;
	j = 0;
	is_in_quote = false;
	while (j < size)
	{
		if ((src[i].c == '\'' || src[i].c == '"') && src[i].is_inter == true)
		{
			if (is_in_quote && quote == src[i].c && ++i)
				is_in_quote = false;
			else if (is_in_quote)
				dst[j++] = src[i++].c;
			else
			{
				is_in_quote = true;
				quote = src[i++].c;
			}
		}
		else
			dst[j++] = src[i++].c;
	}
}

static size_t	get_file_size(const t_char *input)
{
	size_t	i;
	int		tmp;
	size_t	size;

	i = 0;
	size = 0;
	while (input[i].c && (input[i].is_inter == false
			|| (!is_wspace(input[i].c)
				&& !is_redirection(input[i].c) && !is_parenthesis(input[i].c))))
	{
		if ((input[i].c == '\'' || input[i].c == '"')
			&& input[i].is_inter == true)
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
	return (size);
}

char	*get_file_name(t_char *input)
{
	size_t	file_size;
	char	*file_name;

	while (input->is_inter && (input->c == '<'
			|| input->c == '>' || is_wspace(input->c)))
		input++;
	file_size = get_file_size(input);
	file_name = ft_calloc(file_size + 1, sizeof(char));
	if (!file_name)
		return (mem_exh("redirection"), NULL);
	ft_copy(file_name, input, file_size);
	return (file_name);
}

void	delete_file_name(t_char *input)
{
	size_t	size;
	size_t	i;

	size = 0;
	while (input[size].is_inter && (input[size].c == '<'
			|| input[size].c == '>' || is_wspace(input[size].c)))
		size++;
	while (input[size].c && (input[size].is_inter == false
			|| (!is_wspace(input[size].c) && !is_redirection(input[size].c)
				&& !is_parenthesis(input[size].c))))
	{
		if ((input[size].c == '\'' || input[size].c == '"')
			&& input[size].is_inter == true)
			size += skip_quotes_tchar(input + size);
		size++;
	}
	i = 0;
	while (input[size].c)
	{
		input[i].c = input[size].c;
		input[i++].is_inter = input[size++].is_inter;
	}
	input[i].c = 0;
}
