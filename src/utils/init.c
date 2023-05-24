/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:02:20 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/24 18:02:02 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	char		*pwd;
	char		*tmp;

	i = 0;
	len = 0;
	new = ft_calloc(1, sizeof(t_env_info));
	if (new == NULL)
		return (NULL);
	while (env[len] != NULL)			//protect env ??
	{
		len++;
	}
	init_signals(new->act);
	new->fds_heredocs = NULL;
	new->len_heredocs = 0;
	new->len_env = len;
	g_error = 0;
	new->tree = NULL;
	if (ft_getenv((char **)env, "PWD") == -1)
		new->len_env++;
	if (ft_getenv((char **)env, "SHLVL") == -1)
		new->len_env++;
	if (ft_getenv((char **)env, "_") == -1)
		new->len_env++;
	new->env = ft_calloc(new->len_env + 1, sizeof(char *));
	if (new->env == NULL)
		return (free(new), NULL);
	while (i < len)
	{
		new->env[i] = ft_strdup(env[i]);
		if (new->env[i] == NULL)
			return (free_str(new->env), free(new), NULL);
		++i;
	}
	if (ft_getenv((char **)env, "PWD") == -1)
	{
		pwd = getcwd(NULL, 0);
		if (pwd == NULL)
			return (free_str(new->env), free(new), NULL);
		tmp = pwd;
		pwd = ft_strjoin("PWD=", tmp);
		free(tmp);
		new->env[i] = pwd;
		if (new->env[i] == NULL)
			return (free_str(new->env), free(new), NULL);
		++i;
	}
	if (ft_getenv((char **)env, "SHLVL") == -1)
	{
		new->env[i] = ft_strdup("SHLVL=1");
		if (new->env[i] == NULL)
			return (free_str(new->env), free(new), NULL);
		++i;
	}
	if (ft_getenv((char **)env, "_") == -1)
	{
		new->env[i] = ft_strdup("_=");
		if (new->env[i] == NULL)
			return (free_str(new->env), free(new), NULL);
		++i;
	}
	new->env[i] = NULL;
	return (new);
}
