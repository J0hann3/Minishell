/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tchar_join.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 19:24:20 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/19 16:53:39 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_char	*tchar_njoin(t_char *s1, char const *s2, size_t size, bool is_inter)
{
	t_char	*res;
	size_t	len;
	int		j;

	if (!s1)
		len = size;
	else
		len = ft_tchar_len(s1) + size;
	res = malloc(sizeof(t_char) * (len + 1));
	if (res == 0)
		return (NULL);
	j = -1;
	while (s1[++j].c)
	{
		res[j].c = s1[j].c;
		res[j].is_inter = s1[j].is_inter;
	}
	len = 0;
	while (len < size)
	{
		res[j].is_inter = is_inter;
		res[j++].c = s2[len++];
	}
	res[j].c = 0;
	return (res);
}

t_char	*tchar_join(t_char *s1, const t_char *s2)
{
	t_char	*res;
	size_t	len;
	size_t	j;

	len = ft_tchar_len(s1) + ft_tchar_len(s2);
	j = 0;
	res = malloc(sizeof(t_char) * (len + 1));
	if (res == 0)
		return (NULL);
	while (s1[j].c)
	{
		res[j].c = s1[j].c;
		res[j].is_inter = s1[j].is_inter;
		j++;
	}
	len = 0;
	while (s2[len].c)
	{
		res[j].c = s2[len].c;
		res[j++].is_inter = s2[len++].is_inter;
	}
	res[j].c = 0;
	res[j].is_inter = 1;
	return (res);
}
