/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:02:20 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/21 14:22:24 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief	malloc struct and copy env with malloc
 * 			init error and len_env
 * @param env char ** to copy, NULL terminate
 * @return t_env_info * if succes, else return NULL ->error = 2
 */
t_env_info	*init_env(const char **env)
{
	int			i;
	int			len;
	t_env_info	*new;

	i = 0;
	len = 0;
	new = ft_calloc(1, sizeof(t_env_info));
	if (new == NULL)
		return (NULL);
	while (env[len] != NULL)
	{
		len++;
	}
	new->len_env = len;
	new->error = 0;
	new->env = ft_calloc(len + 1, sizeof(char *));
	if (new->env == NULL)
		return (free(new), NULL);
	printf("len_env_init : %d\n", len);
	while (i < len)
	{
		new->env[i] = ft_strdup(env[i]);
		if (new->env[i] == NULL)
			return (free_str(new->env), free(new), NULL);
		++i;
	}
	new->env[i] = NULL;
	return (new);
}

char	**init_arg(const char **arg)
{
	int		i;
	int		len;
	char	**new;

	i = 0;
	len = 0;
	while (arg[len] != NULL)
	{
		len++;
	}
	new = ft_calloc(len + 1, sizeof(char *));
	printf("len_arg_init : %d\n", len);
	if (new == NULL)
		return (NULL);
	while (i < len)
	{
		new[i] = ft_strdup(arg[i]);
		if (new[i] == NULL)
			return (free_str(new), NULL);
		++i;
	}
	new[i] = NULL;
	return (new);
}