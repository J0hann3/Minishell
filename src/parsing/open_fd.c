/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 02:45:22 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/11 17:44:44 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_copy(char *dst, char *src, size_t size)
{
	size_t	i;
	size_t	j;
	char	quote;
	bool	is_in_quote;

	i = 0;
	j = 0;
	quote = 0;
	is_in_quote = false;
	while (j < size)
	{
		if (src[i] == '\'' || src[i] == '"')
		{
			if (is_in_quote && quote == src[i])
				is_in_quote = false;
			else if (is_in_quote)
				dst[j++] = src[i];
			else
			{
				is_in_quote = true;
				quote = src[i];
			}
			i++;
		}
		else
			dst[j++] = src[i++];
	}
	dst[j] = 0;
}

size_t	get_file_size(const char *input)
{
	size_t	i;
	int		tmp;
	size_t	size;

	i = 0;
	size = 0;
	while (input[i] && !is_operator(input + i) && !is_wspace(input[i]) && !is_redirection(input[i]) && !is_parenthesis(input[i]))
	{
		if (input[i] == '\'' || input[i] == '"')
		{
			tmp = skip_quotes(input + i) + 1;
			i += tmp;
			size += tmp - 2;
		}
		else
		{
			size++;
			i++;
		}
	}
	return (size);
}

char	*get_file_name(char *input)
{
	size_t	file_size;
	char	*file_name;

	while (*input == '<' || *input == '>' || is_wspace(*input))
		input++;
	file_size = get_file_size(input);
	file_name = ft_calloc(file_size + 1, sizeof(char));
	if (!file_name)
		return (NULL);
	ft_copy(file_name, input, file_size);
	return (file_name);
}

void delete_file_name(t_char *input)
{
	size_t	size;
	size_t	i;

	size = 0;
	while (input[size].c == '<' || input[size].c == '>' || is_wspace(input[size].c))
		size++;
	while (input[size].c 
		&& !(input[size].c == '|' || (input[size].c == '&' && input[size + 1].c == '&'))
		&& !is_wspace(input[size].c) && !is_redirection(input[size].c))
	{
		if (input[size].c == '\'' || input[size].c == '"')
			size += skip_quotes_tchar(input + size);
		size++;
	}
	i = 0;
	while (input[size].c)
	{
		input[i].c = input[size].c;
		input[i++].is_inter = input[size++].is_inter;

	}
	input[i].c = 0;
}

int	read_fd(t_char *input)
{
	char	*file_name;
	int		fd;

	file_name = ft_tchar_to_str(input);
	if (!file_name)
		return (-1); // write error 
	file_name = get_file_name(file_name);
	if (!file_name)
		return (-1);
	delete_file_name(input);
	fd = open(file_name, O_RDONLY);
	free(file_name);
	return (fd);
}


int	open_fd(t_char *input)
{
	int	fd;
	int	open_mode;
	char	*file_name;

	open_mode = O_CREAT | O_WRONLY | O_TRUNC;
	if (input[0].c == '>' && input[1].c == '>')
		open_mode = O_CREAT | O_WRONLY | O_APPEND;
	file_name = ft_tchar_to_str(input);
	if (!file_name)
		return (-1); // write error 
	file_name = get_file_name(file_name);
	if (!file_name)
		return (-1);
	delete_file_name(input);
	fd = open(file_name, open_mode, 0666);
	free(file_name);
	return (fd);
}


bool	open_all_fds(t_instruction *instruc, t_char *input, int fd_heredocs)
{
	size_t	i;
	bool	is_use_heredoc;

	i = 0;
	is_use_heredoc = false;
	while (input[i].c)
	{
		if ((input[i].c == '\"' || input[i].c == '\'') && input[i].is_inter == true)
			i += skip_quotes_tchar(input + i) + 1;
		else if (input[i].c == '<' && input[i + 1].c != '<')
		{
			if (instruc->infile > -1 && instruc->infile != fd_heredocs)
				close(instruc->infile);
			instruc->infile = read_fd(input + i);
			if (instruc->infile == -1)
			{
				if (fd_heredocs != -1 && is_use_heredoc == false)
					close(fd_heredocs);
				perror("Error"); // change
				return (false);
			}
		}
		else if (input[i].c == '<' && input[i + 1].c == '<')
		{
			if (instruc->infile > -1 && instruc->infile != fd_heredocs)
				close(instruc->infile);
			delete_file_name(input + i);
			if (fd_heredocs != -1)
				(instruc->infile = fd_heredocs, is_use_heredoc = true);
		}
		else if (input[i].c == '>')
		{
			if (instruc->outfile > -1)
				close(instruc->outfile);
			instruc->outfile = open_fd(input + i);
			if (instruc->outfile == -1)
			{
				if (fd_heredocs != -1 && is_use_heredoc == false)
					close(fd_heredocs);
				perror("Error"); // change
				return (false);
			}
		}
		else
			i++;
	}
	return (true);
}

// char	*get_file_name(char *input, size_t *nb_quotes)
// {
// 	size_t	i;
// 	size_t	size;
// 	char	*cpy;
// 	char	quote;
// 	bool	is_in_quote;

// 	cpy = input;
// 	is_in_quote = false;
// 	i = 0;
// 	size = 0;
// 	*nb_quotes = 0;
// 	while (input[i] && !(is_wspace(input[i]) && !is_in_quote)
// 		&& input[i] != '<' && input[i] != '>')
// 	{
// 		if ((input[i] == '"' || input[i] == '\'') && !is_in_quote)
// 		{
// 			quote = input[i++];
// 			is_in_quote = true;
// 			*nb_quotes += 2;
// 		}
// 		else if (is_in_quote && input[i] == quote)
// 		{
// 			is_in_quote = false;
// 			i++;
// 		}
// 		else
// 			cpy[size++] = input[i++];
// 	}
// 	return (ft_strndup(input, size));
// }

// void	replace_name(char **input, size_t size, int offset)
// {
// 	char	*tmp;
// 	size_t	i;
// 	int		j;

// 	i = 0;
// 	tmp = *input;
// 	i = 0;
// 	j = 0;
// 	while ((*input)[i])
// 	{
// 		if ((*input)[i] != '\0' && i < size + 1 + offset)
// 			i++;
// 		else
// 			tmp[j++] = (*input)[i++];
// 	}
// 	tmp[j] = 0;
// 	*input = tmp;
// }

// int	read_fd(char *input)
// {
// 	int		fd;
// 	size_t	nb_quotes;
// 	char	*file_name;
// 	char	*start;
// 	bool	has_space;

// 	start = input;
// 	has_space = false;
// 	while (is_wspace(*++input))
// 		has_space = true;
// 	file_name = get_file_name(input, &nb_quotes);
// 	if (!file_name)
// 		return (mem_exh("open file descriptor"), -1);
// 	printf("input:'%s' len :%ld	len:%ld\n", input, ft_strlen(file_name) - has_space, has_space + nb_quotes);
// 	replace_name(&start, ft_strlen(file_name - has_space), has_space + nb_quotes);
// 	fd = open(file_name, O_RDONLY);
// 	free(file_name);
// 	return (fd);
// }

// bool	heredoc_fd(char *input)
// {
// 	char	*file_name;
// 	char	*start;
// 	size_t	nb_quotes;
// 	bool	has_space;

// 	start = input;
// 	input++;
// 	input++;
// 	has_space = false;
// 	while (is_wspace(*input) && input++)
// 		has_space = true;
// 	file_name = get_file_name(input, &nb_quotes);
// 	if (!file_name)
// 		return (false);
// 	printf("input:'%s' len :%ld	len:%ld\n", input, ft_strlen(file_name) - has_space, has_space + nb_quotes + 1);
// 	replace_name(&start, ft_strlen(file_name - has_space), nb_quotes + has_space + 1);
// 	free(file_name);
// 	return (true);
// }

// int	open_fd(char *input)
// {
// 	int		fd;
// 	int		open_mode;
// 	char	*start;
// 	char	*file_name;
// 	size_t	nb_quotes;
// 	bool	has_space;

// 	start = input;
// 	input++;
// 	has_space = false;
// 	open_mode = O_CREAT | O_WRONLY | O_TRUNC;
// 	if (*input == '>' && input++)
// 		open_mode = O_CREAT | O_WRONLY | O_APPEND;
// 	while (is_wspace(*input) && input++)
// 		has_space = true;
// 	file_name = get_file_name(input, &nb_quotes);
// 	if (!file_name)
// 		return (-1);
// 	printf("input:'%s' len :%ld	len:%ld\n", input,  ft_strlen(file_name) - (has_space + (open_mode & O_APPEND) != 0), has_space + nb_quotes);
// 	replace_name(&start, ft_strlen(file_name - (has_space + (open_mode & O_APPEND) != 0)), has_space + nb_quotes);
// 	fd = open(file_name, open_mode, 0666);
// 	free(file_name);
// 	return (fd);
// }