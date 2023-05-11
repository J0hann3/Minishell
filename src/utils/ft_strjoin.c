/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 12:47:20 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/11 16:58:06 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief join 2 strings, no free
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
	res = malloc(sizeof(char) * (len + 1));
	if (res == 0)
		return (0);
	while (s1 && s1[j])
	{
		res[j] = s1[j];
		j++;
	}
	len = 0;
	while (s2 && s2[len])
	{
		res[j] = s2[len];
		j++;
		len++;
	}
	res[j] = 0;
	return (res);
}

/**
 * @brief join 3 string, no free
 * 
 * @param s1 
 * @param s2 
 * @param s3 
 * @return char* null-terminated
 */
char	*ft_strjoin3(char *s1, char const *s2, char const *s3)
{
	char	*res;
	size_t	len;
	size_t	j;

	len = ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3);
	j = 0;
	res = malloc(sizeof(char) * (len + 1));
	if (res == 0)
		return (0);
	while (s1 && s1[j])
	{
		res[j] = s1[j];
		j++;
	}
	len = 0;
	while (s2 && s2[len])
	{
		res[j] = s2[len];
		j++;
		len++;
	}
	len = 0;
	while (s3 && s3[len])
	{
		res[j] = s3[len];
		j++;
		len++;
	}
	res[j] = 0;
	return (res);
}

/**
 * @brief join a non null-terminated string, free s1
 * 
 * @param s1 
 * @param s2 
 * @param size 
 * @return char* null-terminated
 */
char	*ft_strnjoin(char *s1, char const *s2, size_t size)
{
	char	*res;
	size_t	len;
	size_t	j;

	if (!s1)
		len = size;
	else
		len = ft_strlen(s1) + size;
	j = 0;
	res = malloc(sizeof(char) * (len + 1));
	if (res == 0)
		return (NULL);
	while (s1 && s1[j])
	{
		res[j] = s1[j];
		j++;
	}
	len = 0;
	while (len < size)
		res[j++] = s2[len++];
	res[j] = 0;
	free(s1);
	return (res);
}

char	*ft_strjoin_slash(char *s1, char *s2, int add_slash)
{
	char	*res;
	size_t	len;
	size_t	j;

	len = ft_strlen(s1) + ft_strlen(s2);
	if (add_slash == 1)
		++len;
	res = malloc(sizeof(char) * (len + 1));
	if (res == NULL)
		return (NULL);
	j = -1;
	while (s1 && s1[++j])
		res[j] = s1[j];
	if (add_slash == 1)
	{
		res[j] = '/';
		++j;
	}
	len = -1;
	while (s2 && s2[++len])
	{
		res[j] = s2[len];
		j++;
	}
	res[j] = 0;
	return (res);
}
