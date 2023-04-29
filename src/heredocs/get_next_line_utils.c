/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 01:13:21 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/29 22:06:34 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/get_next_line.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t	i;

	if (!dest && !src)
		return (NULL);
	if (dest < src)
	{
		i = 0;
		while (i < n)
		{
			((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
			i++;
		}
	}
	else
	{
		i = n;
		while (i-- > 0)
			((unsigned char *)dest)[i] = ((unsigned char *)src)[i];
	}
	return (dest);
}

int	lst_add_back(t_buf_list **list, const char *content, size_t read_size)
{
	t_buf_list	*tmp;
	t_buf_list	*added;

	if (!content || !*content)
		return (1);
	added = malloc(sizeof(t_buf_list));
	if (!added)
		return (0);
	ft_memmove(added->string, content, read_size);
	added->string[read_size] = 0;
	added->length = read_size;
	added->next = NULL;
	if (!*list)
	{
		*list = added;
		return (1);
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = added;
	return (1);
}

void	copy_array(char **dest, char (*src)[], size_t size)
{
	size_t	i;

	i = 0;
	while (i < size)
	{
		(*dest)[i] = (*src)[i];
		i++;
	}
}

void	free_list2(t_buf_list **list)
{
	t_buf_list	*cpy;

	if (!list)
		return ;
	while (*list)
	{
		cpy = *list;
		*list = (*list)->next;
		free(cpy);
	}
}
