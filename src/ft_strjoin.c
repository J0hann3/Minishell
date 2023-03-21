/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 12:47:20 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/21 15:20:47 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief join 2 strings and FREE first arg
 * 
 * @param s1 
 * @param s2 
 * @return char*: malloc string terminate by '\0'
 */
char	*ft_strjoin(char *s1, char const *s2)
{
	char	*res;
	size_t	len;
	size_t	j;

	len = ft_strlen(s1) + ft_strlen(s2);
	j = 0;
	res = malloc(sizeof(char) * len + 1);
	if (res == 0)
		return (0);
	while (s1[j])
	{
		res[j] = s1[j];
		j++;
	}
	len = 0;
	while (s2[len])
	{
		res[j] = s2[len];
		j++;
		len++;
	}
	res[j] = 0;
	free(s1);
	return (res);
}

char	*ft_strjoin3(char *s1, char const *s2, char const *s3)
{
	char	*res;
	size_t	len;
	size_t	j;

	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	j = 0;
	res = malloc(sizeof(char) * len + 1);
	if (res == 0)
		return (0);
	while (s1[j])
	{
		res[j] = s1[j];
		j++;
	}
	len = 0;
	while (s2[len])
	{
		res[j] = s2[len];
		j++;
		len++;
	}
	len = 0;
	while (s3[len])
	{
		res[j] = s3[len];
		j++;
		len++;
	}
	res[j] = 0;
	free(s1);
	return (res);
}
