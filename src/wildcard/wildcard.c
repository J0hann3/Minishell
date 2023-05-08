// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   wildcard.c                                         :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2023/05/02 18:27:21 by qthierry          #+#    #+#             */
// /*   Updated: 2023/05/02 19:20:15 by qthierry         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #include "../../includes/minishell.h"

// static bool	expand(char *input, size_t i, char **expanded)
// {
// 	DIR				*dirp;
// 	struct dirent	*dir_context;
	
// 	dirp = opendir(".");
// 	if (!dirp)
// 		return (ft_write_error(NULL, "wildcard", strerror(errno)), false);
	
// 	while (true)
// 	{
// 		dir_context = readdir(dirp);
// 		if (!dir_context)
// 			break ;
// 		printf("%s\n", dir_context->d_name);
// 	}
// 	return (true);
// }

// char	*expand_wild(char *input)
// {
// 	// size_t	i;
// 	// char	*expanded;

// 	// i = 0;
// 	// expanded = NULL;
// 	// while (input[i])
// 	// {
// 	// 	if (input[i] == '"' || input[i] == '\'')
// 	// 		i += skip_quotes(input + i);
// 	// 	else if (input[i] == '*')
// 	// 	{
// 	// 		if(!expand(input, i, &expanded))
// 	// 			return (NULL);
// 	// 		i++;
// 	// 	}
// 	// 	else
// 	// 		i++;
// 	// }
	

// 	return (NULL);
// }