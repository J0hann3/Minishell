/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 02:45:22 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/31 05:06:53 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

char	*get_file_name(char *input)
{
	size_t	i;

	i = 0;
	while (input[i]
		&& !is_wspace(input[i]) && input[i] != '<' && input[i] != '>')
		i++;
	return (ft_strndup(input, i));
}

int	read_fd(char *input, size_t *i)
{
	int		fd;
	char	*file_name;

	(*i)++;
	input++;
	file_name = get_file_name(input);
	if (!file_name)
		return (-1);
	*i += ft_strlen(file_name);
	if (!*file_name)
		return (free(file_name), -2); // ambigous redirect
	fd = open(file_name, O_RDONLY);
	free(file_name);
	return (fd);
}

int	open_fd(char *input, size_t *i)
{
	int		fd;
	int		open_mode;
	char	*file_name;

	(*i)++;
	input++;
	open_mode = O_CREAT;
	if (*input == '>')
		open_mode |= O_APPEND;
	file_name = get_file_name(input);
	if (!file_name)
		return (-1);
	*i += ft_strlen(file_name);
	if (!*file_name)
		return (free(file_name), -2); // ambigous redirect
	printf("file name : %s\n", file_name);
	fd = open(file_name, open_mode, 0666);
	printf("son fd : %d\n", fd);
	free(file_name);
	return (fd);
}


bool	open_all_fds(t_instruction *instruction, char *input)
{
	size_t	i;
	int		in_fd;
	int		out_fd;
	char	buf[10000];

	i = 0;
	in_fd = -1;
	out_fd = -1;
	while (input[i])
	{
		if (input[i] == '\"' || input[i] == '\'')
			i += skip_quotes(input + i);
		else if (input[i] == '<' && input[i + 1] != '<')
		{
			if (in_fd > -1)
				close(in_fd);
			in_fd = read_fd(input + i, &i);
			if (in_fd == -1)
			{
				if (errno == EACCES)
					printf("Permission denied\n");
				else
					printf("%d\nNo such file of directory\n", errno);
				continue ;
			}
		}
		else if (input[i] == '>')
		{
			if (out_fd > -1)
				close(out_fd);
			out_fd = open_fd(input + i, &i);
			if (in_fd == -1)
			{
				if (errno == EACCES)
					printf("Permission denied\n");
				continue ;
			}
			printf("fd sortie : %d\n", out_fd);
		}
		else
			i++;
	}
	printf("fin : in = %d, out = %d\n", in_fd, out_fd);
	if (out_fd > 0)
	{
		printf("fd : %d\n", out_fd);
		write(out_fd, "sortie", 6);
	}
	write(3, "truc", 4);
	return (true);
}