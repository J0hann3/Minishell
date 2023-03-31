/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 02:45:22 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/31 22:50:19 by qthierry         ###   ########.fr       */
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

void	replace_name(char **input, size_t size, bool is_double)
{
	char	*tmp;
	size_t	i;
	size_t	j;

	if (is_double)
		(*input)--;
	(*input)--;
	tmp = *input;
	i = 0;
	j = 0;
	while ((*input)[i])
	{
		if (i < size + 1 + is_double)
			i++;
		else
			tmp[j++] = (*input)[i++];
	}
	tmp[j] = 0;
	*input = tmp;
}

int	read_fd(char *input, size_t *i)
{
	int		fd;
	char	*file_name;

	(*i)++;
	while (is_wspace(*++input))
		(*i)++;
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
	open_mode = O_CREAT | O_WRONLY | O_TRUNC;
	if (*input == '>' && input++)
		open_mode = O_CREAT | O_WRONLY | O_APPEND;
	while (is_wspace(*input) && input++)
		(*i)++;
	file_name = get_file_name(input);
	if (!file_name)
		return (-1);
	replace_name(&input, ft_strlen(file_name), open_mode & O_APPEND);
	*i += ft_strlen(file_name);
	if (!*file_name)
		return (free(file_name), -2); // ambigous redirect
	fd = open(file_name, open_mode, 0666);
	free(file_name);
	return (fd);
}


bool	open_all_fds(t_instruction *instruction, char *input)
{
	size_t	i;
	int		in_fd;
	int		out_fd;

	i = 0;
	in_fd = -2;
	out_fd = -2;
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
			if (out_fd == -1)
			{
				if (errno == EACCES)
					perror("error");
				continue ;
			}
			printf("fd sortie : %d\n", out_fd);
		}
		else
			i++;
	}
	printf("input : '%s'\n", input);
	printf("fin : in = %d, out = %d\n", in_fd, out_fd);
	if (out_fd > 0)
	{
		printf("fd : %d\n", out_fd);
		write(out_fd, "sortie", 6);
	}
	return (true);
}