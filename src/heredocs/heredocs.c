/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/15 18:08:27 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/16 15:59:50 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static bool	is_here_token(const char *input)
{
	return (*input == '<' && *(input + 1) == '<');
}

/**
 * @brief Open the fds of the created heredocs in first
 * parsing and returns them as a chained list
 * @param input 
 * @return t_list *
 */
t_list	*get_here_list(const char *input)
{
	char	*here_ender;
	t_list	*fds_list;
	int		fds[2];
	size_t	i;

	i = 0;
	while (input[i])
	{
		if (is_here_token(input + i))
		{
			// fds = prompt_here(input + i + 2);
			
		}
	}
	return (NULL);
}
