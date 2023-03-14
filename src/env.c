/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 16:04:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/14 17:28:52 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**init_env(const char **env)
{
	int		i;
	int		len;
	char	**new;

	i = 0;
	len = 0;
	while (env[len] != NULL)
	{
		len++;
	}
	new = ft_calloc(len + 1, sizeof(char *));
	printf("len_env_init : %d\n", len + 1);
	if (new == NULL)
		return (NULL);
	while (i < len)
	{
		new[i] = ft_strdup(env[i]);
		if (new[i] == NULL)
			return (free_str(new), NULL);
		++i;
	}
	new[i] = NULL;
	return (new);
}

void	env(char **arg, char **envp)
{
	int	i;

	(void)arg;
	i = 0;
	while (envp[i] != NULL)
	{
		printf("%s\n", envp[i]);
		i++;
	}
}
