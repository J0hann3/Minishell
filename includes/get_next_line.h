/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/19 01:09:59 by qthierry          #+#    #+#             */
/*   Updated: 2023/01/18 17:52:13 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 100
# endif

# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>

typedef struct s_buf_list
{
	char				string[BUFFER_SIZE + 1];
	size_t				length;
	struct s_buf_list	*next;
}	t_buf_list;

void		copy_array(char **dest, char (*src)[], size_t size);
int			get_a_line(int fd, t_buf_list **list, char *buffer);
void		get_new_buffer(t_buf_list **list, int is_end);
char		*get_next_line(int fd);

char		*ft_strchr(const char *s, int c);
void		*ft_memmove(void *dest, const void *src, size_t n);
t_buf_list	*lst_new(const char *content, size_t read_size);
int			lst_add_back(t_buf_list **list, const char *content, \
	size_t read_size);
char		*list_to_str(t_buf_list *list);
void		free_list2(t_buf_list **list);

#endif
