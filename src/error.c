/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 19:26:07 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/20 20:52:55 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_write_error(char *command, char *argument, char *message)
{
	write(2, "minishell: ", 11);
	if (command != NULL)
	{
		write(2, command, ft_strlen(command));
		write(2, ": ", 2);
	}
	if (argument != NULL)
	{
		write(2, argument, ft_strlen(argument));
		write(2, ": ", 2);
	}
	if (message != NULL)
		write(2, message, ft_strlen(message));
	write(2, "\n", 1);
}
