/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tchar_ope.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 00:44:00 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/19 00:48:29 by qthierry         ###   ########.fr       */
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

char	*ft_tchar_to_str(const t_char *input)
{
	size_t	size;
	char	*res;

	size = ft_tchar_len(input);
	res = ft_calloc(size + 1, sizeof(char));
	if (!res)
		return (NULL);
	size = 0;
	while (input[size].c)
	{
		res[size] = input[size].c;
		size++;
	}
	res[size] = 0;
	return (res);
}

t_char	*ft_str_to_tchar(const char *str, bool is_inter)
{
	t_char	*res;
	size_t	i;

	res = ft_calloc(ft_strlen(str) + 1, sizeof(t_char));
	if (!res)
		return (NULL);
	i = 0;
	while (str[i])
	{
		res[i].c = str[i];
		res[i].is_inter = is_inter;
		i++;
	}
	return (res);
}
