/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_list.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/18 17:14:45 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/18 17:15:49 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief Init a file and fills flist with informations.
 * 
 * @param dirp 
 * @return int : -1 if error, 0 if no files are found, 1 if a file is found
 */
int	init_one_file(DIR *dirp, t_file_list **flist, size_t size)
{
	struct dirent	*dir_context;
	t_file_list		*tmp;
	struct stat		s;

	dir_context = readdir(dirp);
	if (!dir_context)
		return (closedir(dirp), 0);
	tmp = ft_realloc((*flist), (size + 1)
			* sizeof(t_file_list), (size + 2) * sizeof(t_file_list));
	if (!tmp)
		return (free_flist((*flist)), closedir(dirp), mem_exh("wildcard"), -1);
	(*flist) = tmp;
	(*flist)[size].file_name
		= ft_strndup(dir_context->d_name, ft_strlen(dir_context->d_name));
	if ((*flist)[size].file_name == NULL)
		return (free_flist((*flist)), closedir(dirp), mem_exh("wildcard"), -1);
	if (stat(dir_context->d_name, &s) == -1)
		return (free_flist((*flist)), closedir(dirp),
			ft_write_error(NULL, "wilcard", strerror(errno)), -1);
	if (S_ISDIR(s.st_mode))
		(*flist)[size].is_dir = 1;
	return (1);
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
	size_t			size;
	int				ret;

	flist = ft_calloc(1, sizeof(t_file_list));
	if (!flist)
		return (mem_exh("wildcard"), NULL);
	dirp = opendir(".");
	if (!dirp)
		return (free_flist(flist),
			ft_write_error(NULL, "wildcard", strerror(errno)), NULL);
	size = 0;
	while (true)
	{
		ret = init_one_file(dirp, &flist, size);
		if (ret == 0)
			return (flist);
		if (ret == -1)
			return (NULL);
		size++;
	}
}

void	free_flist(t_file_list *flist)
{
	size_t	i;

	i = 0;
	if (flist == NULL)
		return ;
	while (flist[i].file_name != NULL)
	{
		free(flist[i].file_name);
		i++;
	}
	free(flist);
}
