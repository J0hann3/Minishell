/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 19:26:07 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/21 14:34:12 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief write messsages of error in stderror in fromat
 * 		"minishell: command: argument: message\n"
 * 
 * @param command char *: Name of command that create error or NULL
 * @param argument char *: Name of argument that create error or NULL
 * @param message char *: Messages of error or NULL
 */
void	ft_write_error(char *command, char *argument, char *message)
{
	char	*str;

	str = ft_strdup("minishell: ");
	// write(2, "minishell: ", 11);
	if (command != NULL)
	{
		str = ft_strjoin(str, command);
		// write(2, command, ft_strlen(command));
		write(2, ": ", 2);
	}
	if (argument != NULL)
	{
		str = ft_strjoin(str, argument);
		// write(2, argument, ft_strlen(argument));
		write(2, ": ", 2);
	}
	if (message != NULL)
	{
		
		str = ft_strjoin(str, message);
		// write(2, message, ft_strlen(message));
	}
	write(2, "\n", 1);
}
