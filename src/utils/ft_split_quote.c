/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 10:35:57 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/19 01:01:06 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/../includes/minishell.h"

void	free_all(t_char **string)
{
	int	i;

	i = 0;
	while (string[i])
	{
		free(string[i]);
		i++;
	}
	free(string);
}

static int	count_words(const t_char *string)
{
	int	res;

	res = 0;
	if (!string->c)
		return (0);
	while (string->c)
	{
		while (is_wspace(string->c))
			string++;
		if (!string->c)
			break ;
		while (string->c)
		{
			if ((string->c == '\'' || string->c == '\"')
				&& string->is_inter == true)
				string += skip_quotes_tchar(string);
			else if (is_wspace(string->c))
				break ;
			string++;
		}
		res++;
	}
	return (res);
}

static t_char	*get_word(const t_char **string)
{
	t_char	*res;
	int		size;
	int		i;

	size = 0;
	while ((*string)->c && is_wspace((*string)->c))
		(*string)++;
	while ((*string)[size].c && !is_wspace((*string)[size].c))
	{
		if (((*string)[size].c == '\'' || (*string)[size].c == '\"')
				&& (*string)[size].is_inter == true)
			size += skip_quotes_tchar((*string) + size);
		size++;
	}
	res = ft_calloc(sizeof(t_char), (size + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (i < size)
	{
		res[i].c = (*string)->c;
		res[i++].is_inter = (*string)++->is_inter;
	}
	res[i].c = 0;
	return (res);
}

t_char	**ft_split_quote(const t_char *string, int *nb_words)
{
	int		i;
	t_char	**res;

	if (!string)
		return (NULL);
	*nb_words = count_words(string);
	res = ft_calloc(sizeof(t_char *), (*nb_words + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (i < *nb_words)
	{
		res[i] = get_word(&string);
		if (!res[i])
			return (free_all(res), NULL);
		i++;
	}
	res[i] = NULL;
	return (res);
}
