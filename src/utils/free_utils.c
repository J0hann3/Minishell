/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/19 00:54:48 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/27 16:25:41 by jvigny           ###   ########.fr       */
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
		free(env->input);
		free(env->fds_heredocs);
		free_str(env->env);
		free_tree(&(env->tree));
		free(env);
	}
	env = NULL;
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
