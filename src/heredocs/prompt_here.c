/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt_here.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 15:42:15 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/25 15:53:15 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*get_warning_message(char *ender)
{
	char	*res;

	res = ft_strjoin3("here-document delimited by end-of-file (wanted `",
			ender, "')");
	return (res);
}

void	prompt_here(char *ender, int fd, char *file_name)
{
	char	*input;

	// open tmp file anyway
	printf(": %s\n", file_name);
	// if (!isatty(STDIN_FILENO) && !isatty(STDERR_FILENO)) // not interactive
	input = (char *)1;
	while (input)
	{
		input = readline("> ");
		if (eq(ender, input))
			break ;
		else if (!input)
		{
			ft_write_error("warning", NULL, get_warning_message(ender)); // rajouter ligne si besoin ;
			break ;
		}
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
	}
	close(fd);
}