/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split_quote.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/04 10:35:57 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/21 15:02:21 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/../includes/minishell.h"

static void	free_all(char **string)
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

static int	count_words(const char *string, char c)
{
	int	res;

	res = 0;
	if (!*string)
		return (1);
	while (*string)
	{
		while (*string == c)
			string++;
		if (!*string)
			break ;
		while (*string)
		{
			if (*string == '\'' || *string == '\"')
				string += skip_quotes(string);
			else if (*string == c)
				break ;
			string++;
		}
		res++;
	}
	return (res);
}

static char	*get_word(const char **string, char c)
{
	char	*res;
	int		size;
	int		i;

	size = 0;
	while (**string && **string == c)
		(*string)++;
	while ((*string)[size] && (*string)[size] != c)
	{
		if ((*string)[size] == '\'' || (*string)[size] == '\"')
			size += skip_quotes((*string) + size);
		size++;
	}
	res = malloc(sizeof(char) * (size + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (i < size)
	{
		res[i] = **string;
		(*string)++;
		i++;
	}
	res[i] = 0;
	return (res);
}

char	**ft_split_quote(const char *string, char c)
{
	int		nb_words;
	int		i;
	char	**res;

	if (!string)
		return (NULL);
	nb_words = count_words(string, c);
	res = malloc(sizeof(char *) * (nb_words + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (i < nb_words)
	{
		res[i] = get_word(&string, c);
		if (!res[i])
			return (free_all(res), NULL);
		i++;
	}
	res[i] = NULL;
	return (res);
}
