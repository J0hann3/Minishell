/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 18:08:27 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/27 11:52:07 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_copy_heredoc(char *dst, char *src, size_t size)
{
	size_t	i;
	size_t	j;
	char	quote;
	bool	is_in_quote;

	i = 0;
	j = 0;
	is_in_quote = false;
	while (j < size)
	{
		if (src[i] == '\'' || src[i] == '"')
		{
			if (is_in_quote && quote == src[i] && ++i)
				is_in_quote = false;
			else if (is_in_quote)
				dst[j++] = src[i++];
			else
			{
				is_in_quote = true;
				quote = src[i++];
			}
		}
		else
			dst[j++] = src[i++];
	}
}

size_t	get_file_size_heredoc(const char *input)
{
	size_t	i;
	int		tmp;
	size_t	size;

	i = 0;
	size = 0;
	while (input[i] && !is_operator(input + i) && !is_wspace(input[i])
		&& !is_redirection(input[i]) && !is_parenthesis(input[i]))
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

char	*get_file_name_heredoc(char *input)
{
	size_t	file_size;
	char	*file_name;

	while (*input == '<' || *input == '>' || is_wspace(*input))
		input++;
	file_size = get_file_size_heredoc(input);
	file_name = ft_calloc(file_size + 1, sizeof(char));
	if (!file_name)
		return (mem_exh("heredocs"), NULL);
	ft_copy_heredoc(file_name, input, file_size);
	file_name[file_size] = 0;
	return (file_name);
}

bool	open_tmp_file(int *fd_r, int *fd_w)
{
	char	*file_name;

	file_name = get_random_name();
	if (!file_name)
		return (false);
	*fd_r = open(file_name, O_CREAT | O_RDONLY, 0644);
	*fd_w = open(file_name, O_CREAT | O_WRONLY, 0644);
	unlink(file_name);
	if (*fd_r < 0 || *fd_w < 0)
	{
		close(*fd_r);
		close(*fd_w);
		return (ft_write_error("heredocs", file_name, strerror(errno)),
			free(file_name), false);
	}
	free(file_name);
	return (true);
}

int	do_here_docs(char *input, t_env_info *env_info)
{
	char	*buffer;
	int		fd_w;
	int		error;

	buffer = get_file_name_heredoc(input);
	if (!buffer)
		return (-1);
	// printf("heredoc end : %s\n",buffer);
	if (!open_tmp_file(env_info->fds_heredocs + env_info->len_heredocs, &fd_w))
		return (free(buffer), -1);
	error = prompt_here(buffer, fd_w, env_info);
	// printf("error : %d\n", error);
	free(buffer);
	return (error);
}
