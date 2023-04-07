/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 02:45:22 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/06 23:45:13 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

char	*get_file_name(char *input)
{
	size_t	i;
	size_t	j;
	char	*cpy;
	char	quote;
	bool	is_in_quote;

	cpy = input;
	is_in_quote = false;
	i = 0;
	j = 0;
	while (input[i] && !(is_wspace(input[i]) && !is_in_quote)
		&& input[i] != '<' && input[i] != '>')
	{
		if ((input[i] == '\"' || input[i] == '\'') && !is_in_quote)
		{
			quote = input[i++];
			is_in_quote = true;
		}
		else if (is_in_quote && input[i] == quote)
		{
			is_in_quote = false;
			i++;
		}
		else
			cpy[j++] = input[i++];
	}
	cpy[j] = 0;
	return (ft_strndup(input, i));
}

void	replace_name(char **input, size_t size, int offset_op)
{
	char	*tmp;
	size_t	i;
	int		j;

	i = 0;
	while (i++ < (size_t)offset_op)
		(*input)--;
	(*input)--;
	tmp = *input;
	i = 0;
	j = 0;
	while ((*input)[i])
	{
		if (i < size + 1 + offset_op)
			i++;
		else
			tmp[j++] = (*input)[i++];
	}
	tmp[j] = 0;
	*input = tmp;
}

int	read_fd(char *input)
{
	int		fd;
	char	*file_name;
	bool	has_space;

	has_space = false;
	while (is_wspace(*++input))
		has_space = true;
	file_name = get_file_name(input);
	if (!file_name)
		return (-1);
	replace_name(&input, ft_strlen(file_name), has_space);
	if (!*file_name)
		return (free(file_name), -2); // ambigous redirect
	fd = open(file_name, O_RDONLY);
	free(file_name);
	return (fd);
}

int	open_fd(char *input)
{
	int		fd;
	int		open_mode;
	char	*file_name;
	bool	has_space;

	input++;
	has_space = false;
	open_mode = O_CREAT | O_WRONLY | O_TRUNC;
	if (*input == '>' && input++)
		open_mode = O_CREAT | O_WRONLY | O_APPEND;
	while (is_wspace(*input) && input++)
		has_space = true;
	file_name = get_file_name(input);
	if (!file_name)
		return (-1);
	replace_name(&input, ft_strlen(file_name), ((open_mode & O_APPEND) != 0) + has_space);
	if (!*file_name)
		return (free(file_name), -2); // ambigous redirect
	fd = open(file_name, open_mode, 0666);
	free(file_name);
	return (fd);
}


bool	open_all_fds(t_instruction *instruc, char *input)
{
	size_t	i;

	i = 0;
	instruc->infile = -2;
	instruc->outfile = -2;
	while (input[i])
	{
		if (input[i] == '\"' || input[i] == '\'')
			i += skip_quotes(input + i) + 1;
		else if (input[i] == '<' && input[i + 1] != '<')
		{
			if (instruc->infile > -1)
				close(instruc->infile);
			instruc->infile = read_fd(input + i);
			if (instruc->infile <= -1)
			{
				perror("Error");
				return (false);
			}
		}
		else if (input[i] == '>')
		{
			if (instruc->outfile > -1)
				close(instruc->outfile);
			instruc->outfile = open_fd(input + i);
			if (instruc->outfile <= -1)
			{
				perror("Error");
				return (false);
			}
			printf("fd sortie : %d\n", instruc->outfile);
		}
		else
			i++;
	}
	return (true);
}