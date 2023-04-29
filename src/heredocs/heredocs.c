/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 18:08:27 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/29 14:24:45 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_here_ender(char *input)
{
	size_t	i;
	size_t	j;
	char	*start;
	char	quote;
	bool	is_in_quote;

	start = input;
	is_in_quote = false;
	i = 0;
	j = 0;
	while (input[i] && !(is_wspace(input[i]) && !is_in_quote)
		&& !is_operator(input) && !is_parenthesis(*input)
		&& !is_redirection(*input))
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
			start[j++] = input[i++];
	}
	return (ft_strndup(input, j));
}

static char	*ft_strjoin_nbr(char *str, int nbr)
{
	char	*res;
	char	*number_str;
	size_t	i;
	size_t	size1;
	size_t	size2;

	number_str = ft_itoa(nbr);
	if (!number_str)
		return (NULL);
	size1 = ft_strlen(str);
	size2 = ft_strlen(number_str);
	res = ft_calloc(size1 + size2 + 1, sizeof(char));
	if (!res)
		return (free(number_str), NULL);
	i = -1;
	while (++i < size1)
		res[i] = str[i];
	i = -1;
	while (++i < size2)
		res[i + size1] = number_str[i];
	return (res);
}

char	*get_next_number(char *previous, int nbr)
{
	char	*start;

	start = previous;
	while (*previous != '_')
		previous++;
	if (ft_strlen(previous) > 55)
		return (NULL);
	previous[1] = 0;
	previous = ft_strjoin_nbr(start, nbr);
	free(start);
	return (previous);
}

char	*get_random_name()
{
	char	*tmp;
	int		exist;
	int		nbr;

	nbr = 1;
	tmp = ft_strdup("./.heredoc_0");
	if (!tmp)
		return (NULL);
	exist = access("/tmp", F_OK);
	if (exist != 0)
		return (ft_write_error("heredocs", "tmp", "No such file or directory"), NULL);
	exist = 1;
	while (true)
	{
		exist = access(tmp, F_OK);
		if (exist == 0)
		{
			tmp = get_next_number(tmp, nbr);
			if (!tmp)
				return (NULL);
		}
		else
			return (tmp);
		tmp = get_next_number(tmp, nbr);
		nbr++;
	}
}

bool	open_tmp_file(char **file_name, int *fd_r, int *fd_w)
{
	*file_name = get_random_name();
	if (!*file_name)
		return (false);
	*fd_r = open(*file_name, O_CREAT | O_RDONLY, 0644);
	*fd_w = open(*file_name, O_CREAT | O_WRONLY, 0644);
	unlink(*file_name);
	if (*fd_r < 0 || *fd_w < 0)
	{
		close(*fd_r);
		close(*fd_w);
		return (ft_write_error("heredocs", *file_name, strerror(errno)), false);
	}
	return (true);
}


int	do_here_docs(char *input, t_env_info *env_info)
{
	char	*buffer;
	char	*file_name;
	int		fd_r;
	int		fd_w;

	buffer = get_here_ender(input);
	if (!buffer)
		return (-1);
	if (!open_tmp_file(&file_name, &fd_r, &fd_w))
		return (-1);
	prompt_here(buffer, fd_w, env_info);
	return (fd_r);
}