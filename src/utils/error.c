/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 19:26:07 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/03 11:07:40 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	ft_write_error_2(const char *command,
			const char *argument, const char *message);

/**
 * @brief write messsages of error in stderror in fromat
 * 		"minishell: command: argument: message\n"
 * 
 * @param command char *: Name of command that create error or NULL
 * @param argument char *: Name of argument that create error or NULL
 * @param message char *: Messages of error or NULL
 */
void	ft_write_error(const char *command,
			const char *argument, const char *message)
{
	char	*str;
	char	*tmp;

	str = ft_strdup("minishell: ");
	if (!str)
		return (ft_write_error_2(command, argument, message));
	if (command != NULL)
	{
		tmp = str;
		str = ft_strjoin3(tmp, command, ": ");
		free(tmp);
		if (!str)
			return (ft_write_error_2(command, argument, message));
	}
	if (argument != NULL)
	{
		tmp = str;
		str = ft_strjoin3(str, argument, ": ");
		free(tmp);
		if (!str)
			return (ft_write_error_2(command, argument, message));
	}
	if (message != NULL)
	{
		tmp = str;
		str = ft_strjoin(str, message);
		free(tmp);
		if (!str)
			return (ft_write_error_2(command, argument, message));
	}
	tmp = str;
	str = ft_strjoin(str, "\n");
	free(tmp);
	if (!str)
		return (ft_write_error_2(command, argument, message));
	write(2, str, ft_strlen(str));
	free(str);
}

void	ft_write_error_2(const char *command,
			const char *argument, const char *message)
{
	write(2, "minishell", 9);
	if (command != NULL)
	{
		write(2, ": ", 2);
		write(2, command, ft_strlen(command));
	}
	if (argument != NULL)
	{
		write(2, ": ", 2);
		write(2, argument, ft_strlen(argument));
	}
	if (message != NULL)
	{
		write(2, ": ", 2);
		write(2, message, ft_strlen(message));
	}
}

void	mem_exh(const char *context)
{
	write(2, "minishell: ", 11);
	if (context)
	{
		write(2, context, ft_strlen(context));
		write(2, ": memory exausted\n", 19);
	}
	else
		write(2, "memory exausted\n", 17);
}
