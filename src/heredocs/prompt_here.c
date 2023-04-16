/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_here.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 15:42:15 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/16 15:59:57 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_ender(const char *input)
{
	char	*ender;

	while (*input == ' ')
		input++;
	ender = (char *)input;
	// while ()
	return (NULL);
}

int	*prompt_here(const char *input)
{
	char	*ender;

	ender = get_ender(input);
	return (NULL);
}