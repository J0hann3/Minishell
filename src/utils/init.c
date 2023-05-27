/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 15:02:20 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/27 16:28:45 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**malloc_env(const char **env, t_env_info *new)
{
	if (ft_getenv((char **)env, "PWD") == -1)
		new->len_env++;
	if (ft_getenv((char **)env, "SHLVL") == -1)
		new->len_env++;
	if (ft_getenv((char **)env, "_") == -1)
		new->len_env++;
	new->env = ft_calloc(new->len_env + 1, sizeof(char *));
	if (new->env == NULL)
		return (free(new), g_error = 1, mem_exh(NULL), NULL);
	return (new->env);
}

static void	add_pwd(t_env_info *new, const char **env, int *i)
{
	char	*pwd;
	char	*tmp;

	if (ft_getenv((char **)env, "PWD") == -1)
	{
		pwd = getcwd(NULL, 0);
		if (pwd == NULL)
			return (free_env(new), g_error = 1, mem_exh(NULL));
		tmp = pwd;
		pwd = ft_strjoin("PWD=", tmp);
		free(tmp);
		new->env[*i] = pwd;
		if (new->env[*i] == NULL)
			return (free_env(new), g_error = 1, mem_exh(NULL));
		++(*i);
	}
}

static void	add_shlvl__(t_env_info *new, const char **env, int *i)
{
	if (ft_getenv((char **)env, "SHLVL") == -1)
	{
		new->env[*i] = ft_strdup("SHLVL=1");
		if (new->env[*i] == NULL)
			return (free_env(new), g_error = 1, mem_exh(NULL));
		++(*i);
	}
	if (ft_getenv((char **)env, "_") == -1)
	{
		new->env[*i] = ft_strdup("_=");
		if (new->env[*i] == NULL)
			return (free_env(new), g_error = 1, mem_exh(NULL));
		++(*i);
	}
}

static void	init_var(t_env_info *new, int len)
{
	init_signals(new->act);
	new->input = NULL;
	new->fds_heredocs = NULL;
	new->len_heredocs = 0;
	new->len_env = len;
	new->tree = NULL;
}

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
		return (g_error = 1, mem_exh(NULL), NULL);
	while (env != NULL && env[len] != NULL)
		len++;
	init_var(new, len);
	if (malloc_env(env, new) == NULL)
		return (NULL);
	while (i < len)
	{
		new->env[i] = ft_strdup(env[i]);
		if (new->env[i] == NULL)
			return (free_env(new), g_error = 1, mem_exh(NULL), NULL);
		++i;
	}
	add_pwd(new, env, &i);
	add_shlvl__(new, env, &i);
	return (new);
}
