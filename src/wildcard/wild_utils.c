/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wild_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 15:05:40 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/11 16:31:37 by qthierry         ###   ########.fr       */
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
bool	is_end_of_single_wildcard(const t_char *input, size_t i)
{
	return (
		(input[i].c == '|' || (input[i].c == '&' && input[i + 1].c == '&'))
		|| is_wspace(input[i].c)
		|| is_redirection(input[i].c) || is_parenthesis(input[i].c)
		|| input[i].c == '*'
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
bool	is_end_of_pattern(const t_char *input, size_t i)
{
	return (
		(input[i].c == '|' || (input[i].c == '&' && input[i + 1].c == '&'))
		|| is_wspace(input[i].c)
		|| is_redirection(input[i].c) || is_parenthesis(input[i].c)
	);
}

/**
 * @brief Returns the pointer where the wildcard pattern starts
 * 
 * @param input 
 * @param start 
 * @return char* 
 */
t_char	*jump_to_pattern_start(const t_char *input, const t_char *start)
{
	char	quote;
	bool	is_in_quote;

	if (input != start)
	{
		input--;
		if (is_end_of_pattern(input, 0))
			return ((t_char *)input + 1);
	}
	is_in_quote = false;
	while (input != start && (!is_end_of_pattern(input, 0) || is_in_quote))
	{
		if ((input->c == '\'' || input->c == '"') && input->is_inter == true)
		{
			if (is_in_quote && quote == input->c)
				is_in_quote = false;
			else if (!is_in_quote)
			{
				is_in_quote = true;
				quote = input->c;
			}
		}
		input--;
	}
	return ((t_char *)input + is_end_of_pattern(input, 0));
}

/**
 * @brief Returns the pointer where the wildcard pattern ends
 * 
 * @param input 
 * @return char* 
 */
t_char	*jump_to_pattern_end(const t_char *input)
{
	char	quote;
	bool	is_in_quote;

	input++;
	is_in_quote = false;
	while (input->c && (!is_end_of_pattern(input, 0) || is_in_quote))
	{
		if ((input->c == '\'' || input->c == '"') && input->is_inter == true)
		{
			if (is_in_quote && quote == input->c)
				is_in_quote = false;
			else if (!is_in_quote)
			{
				is_in_quote = true;
				quote = input->c;
			}
		}
		input++;
	}
	return ((t_char *)input);
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
			return (closedir(dirp), free(flist), mem_exh("wildcard"), NULL);
		flist = tmp;
		flist[size].file_name = dir_context->d_name;
		stat(dir_context->d_name, &s);
		if (S_ISDIR(s.st_mode))
			flist[size].is_dir = 1;
		size++;
	}
}
