/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredocs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 19:43:22 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/29 22:33:29 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/get_next_line.h"

bool	fill_new_fd(int dst_fd, int src_fd)
{
	char	*line;

	line = (char *)1;
	while(line)
	{
		line = get_next_line(src_fd);
		if (!line)
			break ;
		write(dst_fd, line, ft_strlen(line));
		free(line);
	}
	close(src_fd);
	close(dst_fd);
	get_next_line(-1);
	return (true);
}

bool	expand_heredocs(int *fd_in)
{
	int		new_fd_r;
	int		new_fd_w;
	char	*file_name;

	if (*fd_in < 0)
		return (true);
	file_name = get_random_name();
	if (!file_name)
		return (close(*fd_in), false);
	new_fd_r = open(file_name, O_CREAT | O_RDONLY, 0644);
	new_fd_w = open(file_name, O_CREAT | O_WRONLY, 0644);
	unlink(file_name);
	if (new_fd_r < 0 || new_fd_w < 0)
	{
		close(new_fd_r);
		close(new_fd_w);
		return (ft_write_error("heredocs", file_name, strerror(errno)), false);
	}
	if(!fill_new_fd(new_fd_w, *fd_in))
		return (false);
	*fd_in = new_fd_r;
	return (true);
}
