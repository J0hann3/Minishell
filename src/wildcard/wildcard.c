/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 18:27:21 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/08 22:34:06 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_prefix(const char *input, const char *start);
char	*get_suffix(const char *input, bool *is_end);

// int	has_found_patterns(const char *start, size_t wild_pos, char *to_test)
// {
// 	char	*prefix;
// 	char	*suffix;
// 	int		found;

// 	found = 0;
// 	prefix = get_prefix(start + wild_pos, start);
// 	if (!prefix)
// 		return (-1);
// 	suffix = get_suffix(start + wild_pos);
// 	if (!suffix)
// 		return (free(prefix), -1);
// 	if (eqn(prefix, to_test, ft_strlen(prefix))
// 		&& eqn(suffix, to_test - ft_strlen(suffix) + ft_strlen(to_test),
// 			ft_strlen(suffix)))
// 		found = 1;
// 	free(prefix);
// 	free(suffix);
// 	return (found);
// }

// char	*get_start(const char *input, const char *start)
// {
// 	int		i;
// 	char	quote;
// 	bool	is_in_quote;

// 	i = 0;
// 	if (input > start)
// 		i--;
// 	else
// 		return ((char *)input);
// 	while (input + i >= start && !is_operator(input + i) && !is_wspace(input[i])
// 			&& !is_redirection(input[i]) && !is_parenthesis(input[i]) && input[i] != '*')
// 	{
// 		if (input[i] == '/')
// 			return (ft_write_error(NULL, "wildcard", "forbidden `/' in wildcard pattern"), NULL);
// 		if (input[i] == '\'' || input[i] == '"')
// 		{
// 			quote = input[i];
// 			if (input + i <= start)
// 				break ;
// 			i--;
// 			while (input + i >= start && input[i] != quote)
// 			{
// 				if (input[i] == '/')
// 					return (ft_write_error(NULL, "wildcard", "forbidden `/' in wildcard pattern"), NULL);
// 				i--;
// 			}
// 			if (input + i <= start)
// 				break ;
// 			i--;
// 		}
// 		else
// 			i--;
// 	}
// 	return ((char *)input + i + 1);
// }

char	*first_prefix(char *input, t_file_list *flist, const char *start)
{
	char	*to_test;
	char	*tmp;
	int		i;

	i = 0;
	to_test = get_prefix(input, start);
	if (!to_test)
		return (NULL);
	while (flist[i].file_name != NULL)
	{
		tmp = ft_strnstr(flist[i].file_name, to_test, ft_strlen(flist[i].file_name));
		if (!tmp || tmp != flist[i].file_name)
			flist[i].is_matching = 0;
		else
		{
			flist[i].is_matching = 1;
			flist[i].cursor = tmp + ft_strlen(to_test);
		}
		i++;
	}
	free(to_test);
	i = 0;
	while (flist[i].file_name)
	{
		printf("%d : '%s', cursor : '%s'\n", flist[i].is_matching, flist[i].file_name, flist[i].cursor);
		i++;
	}
	// printf("input : '%s'\n", input);
	return (input);
}

char	*suffix(char *input, t_file_list *flist)
{
	char	*suffix;
	char	*tmp;
	int		i;
	bool	is_end;

	i = 0;
	suffix = get_suffix(input, &is_end);
	if (!suffix)
		return (NULL);
	input += ft_strlen(suffix) + 1;
	printf("suffix : '%s', input '%s'\n", suffix, input);
	while (flist[i].file_name != NULL)
	{
		if (flist[i].is_matching == 1 && ft_strlen(suffix) != 0)
		{
			tmp = ft_strnstr(flist[i].cursor, suffix, ft_strlen(flist[i].cursor));
			if (!tmp || (is_end == true && *(tmp + ft_strlen(suffix)) != '\0'))
			{
				if (tmp)
					printf("hey :%d		%s\n", (is_end == true && *(tmp + ft_strlen(suffix)) != '\0'), suffix);
				flist[i].is_matching = 0;
				flist[i].cursor = NULL;
			}
			else
				flist[i].cursor = tmp + ft_strlen(suffix);
		}
		i++;
	}
	free(suffix);
	if (is_end)
		return (NULL);
	
	return (input);
}

void	wildcard(char *input, t_file_list *flist, const char *start)
{
	size_t	i;
	char	*tmp;

	i = 0;
	first_prefix(input, flist, start);
	printf("fgh\n");
	input = suffix(input, flist);
	while (input && *input == '*')
	{
		// printf("truc : '%s'\n", input);
		input = suffix(input, flist);
		// if (*input != '*')
			// break ;
		// input++;
		// if (input[i] == '"' || input[i] == '\'')
		// 	i += skip_quotes(input + i) ;
		// else if (input[i] == '*')
		// i++;
	}
	i = 0;
	while (flist[i].file_name)
	{
		printf("%d : '%s', cursor : '%s'\n", flist[i].is_matching, flist[i].file_name, flist[i].cursor);
		i++;
	}
	// printf("input : '%s'\n", input);
}


t_file_list	*init_flist(t_file_list *flist)
{
	DIR				*dirp;
	struct dirent	*dir_context;
	size_t			size;
	t_file_list 	*tmp;
	
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
		tmp = ft_realloc(flist, (size + 1)* sizeof(t_file_list), (size + 2) * sizeof(t_file_list));
		if (!tmp)
			return (free(flist), NULL);
		flist = tmp;
		flist[size].file_name = dir_context->d_name;
		size++;
	}
}

char	*expand_wild(char *input)
{
	size_t	i;
	char	*expanded;
	t_file_list	*flist;

	i = 0;
	flist = NULL;
	expanded = NULL;
	while (input[i])
	{
		if (input[i] == '"' || input[i] == '\'')
			i += skip_quotes(input + i) + 1;
		else if (input[i] == '*')
		{
			if (!flist)
			{
				flist = init_flist(flist);
				if (!flist)
					return (NULL);
			}
			wildcard(input + i, flist, input);
			break ;
			// if (has_found_patterns(input, i, "\"machin  \"")) // malloc error crash
				// printf("trouve :)\n");
			// else
			// 	printf("pas trouve :(\n");
			// if(!expand(input, i, &expanded))
			// 	return (NULL);
			i++;

			// file1  0
			// file2  1
			// truc   0
			// coucou 0
			// machin 0
			// filia  0
			
			// f*il*e*2 *file1
			// *awd*cou*tru*

		}
		else
			i++;
	}

	return (NULL);
}