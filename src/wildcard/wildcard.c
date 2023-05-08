/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 18:27:21 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/08 17:12:26 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_prefix(const char *start, size_t wild_pos)
{
	size_t	i;
	char	*prefix;

	i = wild_pos;
	while (i > 0)
	{
		if (start[i] == '/')
			return (ft_write_error(NULL, "wildcard", "forbidden `/' in wildcard pattern"), NULL);
		if (is_wspace(start[i]))
			break ;
		i--;
	}
	if (i < 0)
		i = 0;
	prefix = ft_strndup(start + wild_pos - (wild_pos - i), wild_pos - i);
	if (!prefix)
		return (mem_exh("wildcard"), NULL);
	return (prefix);
}

char	*get_suffix(const char *start, size_t wild_pos)
{
	size_t	i;
	char	*suffix;

	i = wild_pos;
	while (start[i])
	{
		if (start[i] == '/')
			return (ft_write_error(NULL, "wildcard", "forbidden `/' in wildcard pattern"), NULL);
		if (is_wspace(start[i]))
			break ;
		i++;
	}
	suffix = ft_strndup(start + wild_pos + 1, i - wild_pos);
	if (!suffix)
		return (mem_exh("wildcard"), NULL);
	return (suffix);
}

int	has_found_patterns(const char *start, size_t wild_pos, char *to_test)
{
	char	*prefix;
	char	*suffix;
	int		found;

	found = 0;
	prefix = get_prefix(start, wild_pos);
	if (!prefix)
		return (-1);
	suffix = get_suffix(start, wild_pos);
	if (!suffix)
		return (free(prefix), -1);
	if (eqn(prefix, to_test, ft_strlen(prefix))
		&& eqn(suffix, to_test - ft_strlen(suffix) + ft_strlen(to_test),
			ft_strlen(suffix)))
		found = 1;
	free(prefix);
	free(suffix);
	return (found);
}


static bool	expand(char *input, size_t i, char **expanded)
{
	DIR				*dirp;
	struct dirent	*dir_context;
	
	dirp = opendir(".");
	if (!dirp)
		return (ft_write_error(NULL, "wildcard", strerror(errno)), false);
	
	while (true)
	{
		dir_context = readdir(dirp);
		if (!dir_context)
			break ;
		printf("%s\n", dir_context->d_name);
	}
	return (true);
}

char	*expand_wild(char *input)
{
	size_t	i;
	char	*expanded;

	i = 0;
	expanded = NULL;
	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
			i += skip_quotes(input + i) + 1;
		else if (input[i] == '*')
		{
			if (has_found_patterns(input, i, "\"machin  \"")) // malloc error crash
				printf("trouve :)\n");
			else
				printf("pas trouve :(\n");
			// if(!expand(input, i, &expanded))
			// 	return (NULL);
			i++;
		}
		else
			i++;
	}

	return (NULL);
}