/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 02:45:22 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/15 22:53:09 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_copy(char *dst, t_char *src, size_t size)
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
		if ((src[i].c == '\'' || src[i].c == '"') && src[i].is_inter == true)
		{
			if (is_in_quote && quote == src[i].c)
				is_in_quote = false;
			else if (is_in_quote)
				dst[j++] = src[i].c;
			else
			{
				is_in_quote = true;
				quote = src[i].c;
			}
			i++;
		}
		else
			dst[j++] = src[i++].c;
	}
	dst[j] = 0;
}

size_t	get_file_size(const t_char *input)
{
	size_t	i;
	int		tmp;
	size_t	size;

	i = 0;
	size = 0;
	while (input[i].c && (input[i].is_inter == false
		|| (!is_wspace(input[i].c) && !is_redirection(input[i].c) && !is_parenthesis(input[i].c))))
	{
		if ((input[i].c == '\'' || input[i].c == '"') && input[i].is_inter == true)
		{
			tmp = skip_quotes_tchar(input + i) + 1;
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

char	*get_file_name(t_char *input)
{
	size_t	file_size;
	char	*file_name;

	while (is_inter_and_eq(*input, '<') || is_inter_and_eq(*input, '>')
		|| is_inter_and_eq(*input, ' ') || is_inter_and_eq(*input, '\t'))
		input++;
	file_size = get_file_size(input);
	file_name = ft_calloc(file_size + 1, sizeof(char));
	if (!file_name)
		return (mem_exh("redirection"), NULL);
	ft_copy(file_name, input, file_size);
	return (file_name);
}

void delete_file_name(t_char *input)
{
	size_t	size;
	size_t	i;

	size = 0;
	while (is_inter_and_eq(input[size], '<') || is_inter_and_eq(input[size], '>')
			|| is_inter_and_eq(input[size], ' ') || is_inter_and_eq(input[size], '\t'))
		size++;
	while (input[size].c && (input[size].is_inter == false
			|| (!is_wspace(input[size].c) && !is_redirection(input[size].c) && !is_parenthesis(input[size].c))))
	{
		if ((input[size].c == '\'' || input[size].c == '"') && input[size].is_inter == true)
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
	int	fd;
	int	open_mode;
	char	*file_name;

	open_mode = O_CREAT | O_WRONLY | O_TRUNC;
	if (is_inter_and_eq(input[0], '>') && is_inter_and_eq(input[1], '>'))
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
		else if (input[i].c == '<' && input[i + 1].c == '<' 
				&& input[i].is_inter == true && input[i + 1].is_inter == true)
		{
			if (instruc->infile > -1 && instruc->infile != fd_heredocs)
				close(instruc->infile);
			delete_file_name(input + i);
			if (fd_heredocs != -1)
				(instruc->infile = fd_heredocs, is_use_heredoc = true);
			else
			{
				if (fd_heredocs > -1 && is_use_heredoc == false)
					close(fd_heredocs);
				return (false);
			}
		}
		else if (input[i].c == '<' && input[i].is_inter == true)
		{
			if (instruc->infile > -1 && instruc->infile != fd_heredocs)
				close(instruc->infile);
			if (instruc->infile == fd_heredocs)
				is_use_heredoc = false;
			instruc->infile = read_fd(input + i);
			if (instruc->infile == -1)
			{
				if (fd_heredocs > -1 && is_use_heredoc == false)
					close(fd_heredocs);
				return (false);
			}
		}
		else if (input[i].c == '>' && input[i].is_inter == true)
		{
			if (instruc->outfile > -1)
				close(instruc->outfile);
			instruc->outfile = open_fd(input + i);
			if (instruc->outfile == -1)
			{
				if (fd_heredocs > -1 && is_use_heredoc == false)
					close(fd_heredocs);
				return (false);
			}
		}
		else
			i++;
	}
	if (fd_heredocs > -1 && is_use_heredoc == false)
		close(fd_heredocs);
	return (true);
}
