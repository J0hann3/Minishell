/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/14 16:59:33 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/15 18:48:57 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief free char ** while != NULL
 * 
 * @param str char **: terminate by NULL -> need to by free
 */
void	free_str(char **str)
{
	int	i;

	i = 0;
	if (str == NULL || *str == NULL)
		return (free(str));
	while (str[i] != NULL)
	{
		free(str[i]);
		++i;
	}
	free(str);
}

void	free_env(t_env_info *env)
{
	if (env != NULL)
	{
		free(env->fds_heredocs);
		free_str(env->env);
		free_tree(&(env->tree));
		free(env);
	}
	env = NULL;
}

int	is_alpha(char c)
{
	if (c >= 'a' && c <= 'z')
		return (1);
	if (c >= 'A' && c <= 'Z')
		return (1);
	return (0);
}

int	is_digit(char c)
{
	if (c >= '0' && c <= '9')
		return (1);
	return (0);
}

/**
 * @brief search str in the env
 * 
 * @param env char **: terminate by NULL ->where to search variable 
 * @param str char *: Name of variable to find
 * @return int index where is find the variable, if not find return -1
 */
int	ft_getenv(char **env, char *str)
{
	int	i;
	int	j;

	i = 0;
	while (env[i] != NULL)
	{
		j = 0;
		while (env[i][j] != '\0')
		{
			if (env[i][j] == '=' && str[j] == '\0')
				return (i);
			if (str[j] == '\0')
				break ;
			if (str[j] != env[i][j])
				break ;
			++j;
		}
		++i;
	}
	return (-1);
}

void	close_fd_heredocs(t_env_info *env)
{
	int	i;

	i = 0;
	while (i <= env->len_heredocs)
	{
		if (env->fds_heredocs[i] >= 0)
			close(env->fds_heredocs[i]);
		i++;
	}
	free(env->fds_heredocs);
	env->fds_heredocs = NULL;
	env->len_heredocs = 0;
}

void	close_fd(t_instruction *inst)
{
	if (inst == NULL)
		return ;
	if (inst->infile >= 0)
		close(inst->infile);
	if (inst->outfile >= 0)
		close(inst->outfile);
	if (inst->s_infile >= 0)
		close(inst->s_infile);
	if (inst->s_outfile >= 0)
		close(inst->s_outfile);
}
