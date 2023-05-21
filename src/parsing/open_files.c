/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/21 02:55:38 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/21 02:58:01 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	read_fd(t_char *input)
{
	char	*file_name;
	int		fd;

	file_name = get_file_name(input);
	if (!file_name)
		return (-1);
	delete_file_name(input);
	fd = open(file_name, O_RDONLY);
	if (fd == -1)
		ft_write_error(NULL, file_name, strerror(errno));
	free(file_name);
	return (fd);
}

int	open_fd(t_char *input)
{
	int		fd;
	int		open_mode;
	char	*file_name;

	open_mode = O_CREAT | O_WRONLY | O_TRUNC;
	if (inter_eq(input[0], '>') && inter_eq(input[1], '>'))
		open_mode = O_CREAT | O_WRONLY | O_APPEND;
	file_name = get_file_name(input);
	if (!file_name)
		return (-1);
	delete_file_name(input);
	fd = open(file_name, open_mode, 0666);
	if (fd == -1)
		ft_write_error(NULL, file_name, strerror(errno));
	free(file_name);
	return (fd);
}
