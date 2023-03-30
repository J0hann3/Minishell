/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 15:39:52 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/20 20:15:57 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	nb_mot(char const *s, char c)
{
	int		res;
	size_t	last_rep;
	size_t	i;

	res = 0;
	i = 0;
	last_rep = -1;
	if (s == 0)
		return (0);
	while (s[i])
	{
		if (s[i] == c)
		{
			if ((i - 1) != last_rep)
				res++;
			last_rep = i;
		}
		i++;
	}
	if (i == ft_strlen(s) && (i - 1) != last_rep)
		res++;
	return (res);
}

static int	index_mot(char const *s, char c, int n, int *len)
{
	int	j;
	int	last_rep;
	int	i;

	j = 0;
	i = -1;
	last_rep = -1;
	while (s[++i])
	{
		if (s[i] == c)
		{
			if ((i - 1) != last_rep)
				j++;
			last_rep = i;
		}
		else if (j == n)
		{
			j = 0;
			while (s[i + j] != c && s[i + j])
				j++;
			*len = j;
			return (i);
		}
	}
	return (0);
}

static char	*cpy_mot(char const *s, int index, int len_mot)
{
	char	*res;
	int		i;

	i = 0;
	res = malloc(sizeof(char) * (len_mot + 1));
	if (res == 0)
		return (0);
	while (i < len_mot)
	{
		res[i] = s[index];
		i++;
		index++;
	}
	res[i] = 0;
	return (res);
}

static char	**free_all(int i, char **res)
{
	while (i >= 0)
	{
		free(res[i]);
		i--;
	}
	free(res);
	return (0);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	int		i;
	int		nb;
	int		index;
	int		len_mot;

	nb = nb_mot(s, c);
	i = -1;
	res = malloc(sizeof(char *) * (nb + 1));
	if (res == 0)
	{
		free(res);
		return (0);
	}
	while (++i < nb)
	{
		index = index_mot(s, c, i, &len_mot);
		res[i] = cpy_mot(s, index, len_mot);
		if (res[i] == 0)
			return (free_all(i, res));
	}
	res[i] = 0;
	return (res);
}
