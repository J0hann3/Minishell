/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 02:45:22 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/29 17:30:38 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_file_name(char *input, size_t *size)
{
	size_t	i;
	char	*cpy;
	char	quote;
	bool	is_in_quote;

	cpy = input;
	is_in_quote = false;
	i = 0;
	(*size) = 0;
	while (input[i] && !(is_wspace(input[i]) && !is_in_quote)
		&& input[i] != '<' && input[i] != '>')
	{
		if ((input[i] == '"' || input[i] == '\'') && !is_in_quote)
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
			cpy[(*size)++] = input[i++];
	}
	// cpy[(*size)] = 0;
	return (ft_strndup(input, (*size)));
}

void	replace_name(char **input, size_t size, int offset_op)
{
	char	*tmp;
	size_t	i;
	int		j;

	// printf("input : '%s'\n", *input);
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
	// printf("tmp : '%s'\n", tmp);
}

int	read_fd(char *input)
{
	int		fd;
	size_t		size;
	char	*file_name;
	bool	has_space;

	has_space = false;
	while (is_wspace(*++input))
		has_space = true;
	file_name = get_file_name(input, &size);
	if (!file_name)
		return (-1);
	file_name[size] = 0;
	// printf("command before read : '%s'	[%ld]\n", input, ft_strlen(file_name));
	replace_name(&input, ft_strlen(file_name), has_space);
	fd = open(file_name, O_RDONLY);
	// printf("command after read : '%s'\n", input);
	free(file_name);
	return (fd);
}

bool	heredoc_fd(char *input)
{
	char	*file_name;
	size_t		size;
	bool	has_space;

	input++;
	input++;
	has_space = false;
	while (is_wspace(*input) && input++)
		has_space = true;
	file_name = get_file_name(input, &size);
	if (!file_name)
		return (false);
	file_name[size] = 0;
	// printf("command before heredoc : '%s'	[%ld]\n", input, ft_strlen(file_name));
	replace_name(&input, ft_strlen(file_name), 1 + has_space);
	// printf("command after heredoc : '%s'\n", input);
	free(file_name);
	return (true);
}

int	open_fd(char *input)
{
	int		fd;
	int		open_mode;
	char	*file_name;
	size_t		size;
	bool	has_space;

	input++;
	has_space = false;
	open_mode = O_CREAT | O_WRONLY | O_TRUNC;
	if (*input == '>' && input++)
		open_mode = O_CREAT | O_WRONLY | O_APPEND;
	while (is_wspace(*input) && input++)
		has_space = true;
	file_name = get_file_name(input, &size);
	if (!file_name)
		return (-1);
	file_name[size] = 0;
	// printf("command before open : '%s'	[%ld]	%d\n", input, ft_strlen(file_name), ((open_mode & O_APPEND) != 0) + has_space);
	replace_name(&input, ft_strlen(file_name), ((open_mode & O_APPEND) != 0) + has_space);
	fd = open(file_name, open_mode, 0666);
	// printf("command after open : '%s'\n", input);
	free(file_name);
	return (fd);
}


bool	open_all_fds(t_instruction *instruc, char *input, int fd_heredocs)
{
	size_t	i;

	i = 0;
	instruc->infile = -2;
	instruc->outfile = -2;
	while (input[i])
	{
		// printf("'%s'\n", input);
		if (input[i] == '\"' || input[i] == '\'')
			i += skip_quotes(input + i) + 1;
		else if (input[i] == '<' && input[i + 1] != '<')
		{
			if (instruc->infile > -1)
				close(instruc->infile);
			instruc->infile = read_fd(input + i);
			if (instruc->infile == -1)
			{
				perror("Error");
				return (false);
			}
		}
		else if (input[i] == '<' && input[i + 1] == '<')
		{
			if (instruc->infile > -1)
				close(instruc->infile);
			if (!heredoc_fd(input + i))
			{
				perror("Error");
				return (false);
			}
			if (fd_heredocs != -1)
				instruc->infile = fd_heredocs;
		}
		else if (input[i] == '>')
		{
			if (instruc->outfile > -1)
				close(instruc->outfile);
			instruc->outfile = open_fd(input + i);
			if (instruc->outfile == -1)
			{
				perror("Error");
				return (false);
			}
		}
		else
			i++;
	}
	return (true);
}
