/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 19:26:07 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/02 17:30:59 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
		return ;
	if (command != NULL)
	{
		tmp = str;
		str = ft_strjoin3(tmp, command, ": ");
		free(tmp);
	}
	if (argument != NULL)
	{
		tmp = str;
		str = ft_strjoin3(str, argument, ": ");
		free(tmp);
	}
	if (message != NULL)
	{
		tmp = str;
		str = ft_strjoin(str, message);
		free(tmp);
	}
	tmp = str;
	str = ft_strjoin(str, "\n");
	if (!str)
		mem_exh("error print");
	free(tmp);
	write(2, str, ft_strlen(str));
	free(str);
}

void	mem_exh(const char *context)
{
	write(2, "minishell: ", 11);
	write(2, context, ft_strlen(context));
	write(2, ": memory exausted\n", 19);
}
