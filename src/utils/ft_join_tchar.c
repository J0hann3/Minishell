/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_join_tchar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 19:24:20 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/10 20:31:17 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


size_t	ft_tchar_len(const t_char *s)
{
	size_t	i;

	i = 0;
	while (s[i].c)
		i++;
	return (i);
}

t_char	*ft_tchar_njoin(t_char *s1, char const *s2, size_t size, bool is_inter)
{
	t_char	*res;
	size_t	len;
	size_t	j;

	if (!s1)
		len = size;
	else
		len = ft_tchar_len(s1) + size;
	j = 0;
	res = malloc(sizeof(t_char) * len + 1);
	if (res == 0)
		return (NULL);
	while (s1[j].c)
	{
		res[j].c = s1[j].c;
		res[j].is_inter = s1[j].is_inter;
		j++;
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

t_char	*ft_tchar_join(t_char *s1, const t_char *s2)
{
	t_char	*res;
	size_t	len;
	size_t	j;

	len = ft_tchar_len(s1) + ft_tchar_len(s2);
	j = 0;
	res = malloc(sizeof(t_char) * len + 1);
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
		res[j].is_inter = s2[len].is_inter;
		res[j++].c = s2[len++].c;
	}
	res[j].c = 0;
	res[j].is_inter = 1;
	return (res);
}

size_t	skip_quotes_tchar(const t_char *input)
{
	char	quote;
	size_t	i;

	quote = input->c;
	i = 1;
	while (input[i].c)
	{
		if (input[i].c == quote && input[i].is_inter == true)
			return (i);
		i++;
	}
	return (0);
}

t_char	*ft_tcharndup(const t_char *s, size_t n)
{
	t_char	*res;
	size_t	i;

	res = malloc(sizeof(t_char) * (n + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (s[i].c && i < n)
	{
		res[i].c = ((t_char *)s)[i].c;
		res[i].is_inter = ((t_char *)s)[i].is_inter;
		i++;
	}
	res[i].c = 0;
	return (res);
}