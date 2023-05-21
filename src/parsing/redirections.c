/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 02:45:22 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/21 02:57:58 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	on_heredoc(t_instruction *instruc,
	t_char *input, int fd_heredoc, bool *is_use_heredoc)
{
	if (instruc->infile > -1 && instruc->infile != fd_heredoc)
		close(instruc->infile);
	delete_file_name(input);
	if (fd_heredoc != -1)
	{
		instruc->infile = fd_heredoc;
		*is_use_heredoc = true;
	}
	else
	{
		if (fd_heredoc > -1 && *is_use_heredoc == false)
			close(fd_heredoc);
		return (false);
	}
	return (true);
}

static bool	on_read_fd(t_instruction *instruc,
	t_char *input, int fd_heredoc, bool *is_use_heredoc)
{
	if (instruc->infile > -1 && instruc->infile != fd_heredoc)
		close(instruc->infile);
	if (instruc->infile == fd_heredoc)
		*is_use_heredoc = false;
	instruc->infile = read_fd(input);
	if (instruc->infile == -1)
	{
		if (fd_heredoc > -1 && *is_use_heredoc == false)
			close(fd_heredoc);
		return (false);
	}
	return (true);
}

static bool	on_open_fd(t_instruction *instruc,
	t_char *input, int fd_heredoc, bool *is_use_heredoc)
{
	if (instruc->outfile > -1)
		close(instruc->outfile);
	instruc->outfile = open_fd(input);
	if (instruc->outfile == -1)
	{
		if (fd_heredoc > -1 && *is_use_heredoc == false)
			close(fd_heredoc);
		return (false);
	}
	return (true);
}

/**
 * @brief Returns -1 on fatal error, 0 on something happened,
 * 1 on nothing happened
 * 
 * @param instruc 
 * @param input 
 * @param fd_heredoc 
 * @param is_use_heredoc 
 * @return int 
 */
static int	test_for_redirect(t_instruction *instruc,
		t_char *input, int fd_heredoc, bool *is_use_heredoc)
{
	if (inter_eq(input[0], '<') && inter_eq(input[1], '<'))
	{
		if (!on_heredoc(instruc, input, fd_heredoc, is_use_heredoc))
			return (-1);
		return (0);
	}
	else if (inter_eq(*input, '<'))
	{
		if (!on_read_fd(instruc, input, fd_heredoc, is_use_heredoc))
			return (-1);
		return (0);
	}
	else if (inter_eq(*input, '>'))
	{
		if (!on_open_fd(instruc, input, fd_heredoc, is_use_heredoc))
			return (-1);
		return (0);
	}
	return (1);
}

bool	open_all_fds(t_instruction *instruc, t_char *input, int fd_heredoc)
{
	size_t	i;
	bool	is_use_heredoc;
	int		ret;

	i = 0;
	is_use_heredoc = false;
	while (input[i].c)
	{
		if ((input[i].c == '\"' || input[i].c == '\'')
			&& input[i].is_inter == true)
			i += skip_quotes_tchar(input + i) + 1;
		else
		{
			ret = test_for_redirect(instruc, input + i,
					fd_heredoc, &is_use_heredoc);
			if (ret == -1)
				return (false);
			if (ret == 1)
				i++;
		}
	}
	if (fd_heredoc > -1 && is_use_heredoc == false)
		close(fd_heredoc);
	return (true);
}
