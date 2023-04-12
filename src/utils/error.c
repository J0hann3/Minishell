/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 19:26:07 by jvigny            #+#    #+#             */
/*   Updated: 2023/04/12 15:52:14 by jvigny           ###   ########.fr       */
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
void	ft_write_error(char *command, char *argument, char *message)
{
	char	*str;
	char	*tmp;

	str = ft_strdup("minishell: ");
	// write(2, "minishell: ", 11);
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
	free(tmp);
	write(2, str, ft_strlen(str));
	free(str);
}

void	free_all(t_env_info *env, char **arg)
{
	if (arg != NULL)
		free_str(arg);
	free_str(env->env);
	free_tree(env->tree);
	free(env);
}