/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 18:08:27 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/22 21:32:42 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_here_ender(char *input)
{
	char	*start;
	char	*ender;

	start = input;
	while (*input && !is_wspace(*input) && !is_operator(input)
			&& !is_parenthesis(*input) && !is_redirection(*input))
		input++;
	ender = ft_strndup(start, input - start);
	return (ender);
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
	char	*random_name;
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

int	open_tmp_file(char **file_name)
{
	int		fd;

	*file_name = get_random_name();
	if (!*file_name)
		return (-1); // error creating tmp file
	fd = open(*file_name, O_CREAT | O_WRONLY, 0644);
	if (fd < 0)
		return (ft_write_error("heredocs", *file_name, strerror(errno)), -1);
	return (fd);
}


int	do_here_docs(char *input)
{
	char	*buffer;
	char	*file_name;
	int		fd;

	buffer = get_here_ender(input);
	if (!buffer)
		return (-1);
	int pid = fork();
	if (pid == 0)
	{
		fd = open_tmp_file(&file_name);
		if (fd == -1)
			return (-1);
		fd = prompt_here(buffer, fd, file_name);
		exit(0);
	}
	waitpid(pid, NULL, 0);
	
	//expand
	return (fd);
}