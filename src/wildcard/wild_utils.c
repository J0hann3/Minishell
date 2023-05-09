/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 15:05:40 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/09 18:03:04 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Does include the wildcard in the stop condition
 * 
 * @param input 
 * @param i 
 * @return true 
 * @return false 
 */
bool	is_end_of_single_wildcard(const char *input, size_t i)
{
	return (
		is_operator(input + i) && is_wspace(input[i])
		&& is_redirection(input[i]) && is_parenthesis(input[i])
		&& input[i] == '*'
	);
}

/**
 * @brief Does not include wildcard, used for getting the end of a pattern
 * 
 * @param input 
 * @param i 
 * @return true 
 * @return false 
 */
bool	is_end_of_pattern(const char *input, size_t i)
{
	return (
		is_operator(input + i) && is_wspace(input[i])
		&& is_redirection(input[i]) && is_parenthesis(input[i])
	);
}

/**
 * @brief Returns the pointer where the wildcard pattern starts
 * 
 * @param input 
 * @param start 
 * @return char* 
 */
char	*jump_to_pattern_start(const char *input, const char *start)
{
	char	quote;
	bool	is_in_quote;

	if (input != start)
		input--;
	is_in_quote = false;
	while (input != start && (!is_end_of_pattern(input, 0) || is_in_quote))
	{
		if (*input == '\'' || *input == '"')
		{
			if (is_in_quote && quote == *input)
				is_in_quote = false;
			else
			{
				is_in_quote = true;
				quote = *input;
			}
		}
		input--;
	}
	return ((char *)input);
}

/**
 * @brief Returns the pointer where the wildcard pattern ends
 * 
 * @param input 
 * @return char* 
 */
char	*jump_to_pattern_end(const char *input)
{
	char	quote;
	bool	is_in_quote;

	input++;
	is_in_quote = false;
	while (*input && (!is_end_of_pattern(input, 0) || is_in_quote))
	{
		if (*input == '\'' || *input == '"')
		{
			if (is_in_quote && quote == *input)
				is_in_quote = false;
			else
			{
				is_in_quote = true;
				quote = *input;
			}
		}
		input++;
	}
	return ((char *)input);
}

/**
 * @brief Init the file name list once by reading in the current directory
 * and getting all the files name in the file_list struct.
 * 
 * @param flist 
 * @return t_file_list* 
 */
t_file_list	*init_flist(t_file_list *flist)
{
	DIR				*dirp;
	struct dirent	*dir_context;
	size_t			size;
	t_file_list 	*tmp;
	struct stat		s;

	flist = ft_calloc(1, sizeof(t_file_list));
	if (!flist)
		return (false);
	dirp = opendir(".");
	if (!dirp)
		return (free(flist), ft_write_error(NULL, "wildcard", strerror(errno)), NULL);
	size = 0;
	while (true)
	{
		dir_context = readdir(dirp);
		if (!dir_context)
			return (flist);
		tmp = ft_realloc(flist, (size + 1) * sizeof(t_file_list), (size + 2) * sizeof(t_file_list));
		if (!tmp)
			return (free(flist), mem_exh("wildcard"), NULL);
		flist = tmp;
		flist[size].file_name = dir_context->d_name;
		stat(dir_context->d_name, &s);
		if (S_ISDIR(s.st_mode))
			flist[size].is_dir = 1;
		size++;
	}
}
