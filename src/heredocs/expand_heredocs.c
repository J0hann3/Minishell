/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_heredocs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/29 19:43:22 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/13 18:15:57 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"
#include "../../includes/get_next_line.h"
#include <unistd.h>

static char	*expand(char *input, size_t *i, t_env_info *env_info)
{
	char	*tmp;
	int		env_index;
	size_t	j;
	size_t	size;

	if (*input == '?')
	{
		(*i)++;
		return (ft_itoa(g_error));
	}
	size = get_size_of_var(input);
	*i += size;
	tmp = ft_strndup(input, size);
	if (!tmp)
		return (NULL);
	env_index = ft_getenv(env_info->env, tmp);
	free(tmp);
	if (env_index == -1)
		return (ft_calloc(1, sizeof(char)));
	tmp = env_info->env[env_index];
	j = 0;
	while (tmp[j] && tmp[j] != '=')
		j++;
	tmp = ft_strdup(tmp + j + 1);
	return (tmp);
}

char	*expand_here(char *input, t_env_info *env_info)
{
	size_t	i;
	int		begin_join;
	char	*tmp;
	char	*res;

	res = ft_calloc(1, sizeof(char));
	if (!res)
		return (free(input), NULL);
	i = 0;
	begin_join = 0;
	while (input[i])
	{
		if (input[i] == '$' && input[i + 1] != '$' && is_expandable(input + i + 1))
		{
			if (i > 0)
				res = ft_strnjoin(res, input + begin_join, i - begin_join);
			i++;
			tmp = expand(input + i, &i, env_info);
			if (!tmp)
				return (free(res), free(input), NULL);
			begin_join = i;
			res = ft_strnjoin(res, tmp, ft_strlen(tmp));
			free(tmp);
		}
		else
			i++;
	}
	if (begin_join != (int)i)
		res = ft_strnjoin(res, input + begin_join, i - begin_join);
	free(input);
	return (res);
}


static void	fill_new_fd(int dst_fd, int src_fd, t_env_info *env_info)
{
	char	*line;

	while(true)
	{
		line = get_next_line(src_fd);
		if (!line)
			break ;
		line = expand_here(line, env_info);
		if (!line)
			break ;
		write(dst_fd, line, ft_strlen(line));
		free(line);
	}
	close(src_fd);
	close(dst_fd);
	get_next_line(-1);
}

bool	expand_heredocs(int *fd_in, t_env_info *env_info)
{
	int		new_fd_r;
	int		new_fd_w;
	char	*file_name;

	if (*fd_in < 0)
		return (true);
	file_name = get_random_name();
	if (!file_name)
		return (close(*fd_in), false);
	new_fd_r = open(file_name, O_CREAT | O_RDONLY, 0644);
	new_fd_w = open(file_name, O_CREAT | O_WRONLY, 0644);
	unlink(file_name);
	if (new_fd_r < 0 || new_fd_w < 0)
	{
		close(new_fd_r);
		close(new_fd_w);
		return (close(*fd_in), ft_write_error("heredocs", file_name, strerror(errno)), free(file_name), false);
	}
	free(file_name);
	fill_new_fd(new_fd_w, *fd_in, env_info);
	*fd_in = new_fd_r;
	return (true);
}
