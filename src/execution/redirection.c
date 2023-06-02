/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 16:12:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/02 18:41:53 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	redirection(t_instruction *inst)
{
	if (inst->infile >= 0)
	{
		close(inst->infile);
		inst->infile = -2;
	}
	if (inst->outfile >= 0)
	{
		inst->s_outfile = dup(STDOUT_FILENO);
		if (inst->s_outfile == -1)
			return (g_error = 1, ft_write_error("file", NULL, strerror(errno)));
		if (dup2(inst->outfile, STDOUT_FILENO) == -1)
		{
			close(inst->s_outfile);
			inst->s_outfile = -2;
			g_error = 1;
			ft_write_error(NULL, NULL, strerror(errno));
		}
		close(inst->outfile);
		inst->outfile = -2;
	}
}

void	reset_redirection(t_instruction *inst)
{
	if (inst->s_outfile >= 0)
	{
		if (dup2(inst->s_outfile, STDOUT_FILENO) == -1)
		{
			g_error = 1;
			ft_write_error(NULL, NULL, strerror(errno));
		}
		close(inst->s_outfile);
		inst->s_outfile = -2;
	}
}

void	redirection_fork(t_instruction *inst)
{
	if (inst->infile >= 0)
	{
		if (dup2(inst->infile, STDIN_FILENO) == -1)
		{
			g_error = 1;
			ft_write_error(NULL, NULL, strerror(errno));
		}
		close(inst->infile);
		inst->infile = -2;
	}
	if (inst->outfile >= 0)
	{
		if (dup2(inst->outfile, STDOUT_FILENO) == -1)
		{
			g_error = 1;
			ft_write_error(NULL, NULL, strerror(errno));
		}
		close(inst->outfile);
		inst->outfile = -2;
	}
}

void	redirect_infile_pipe(int fd, t_env_info *env)
{
	get_next_line(-1);
	if (fd != 0)
	{
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			free_env(env);
			g_error = 1;
			close(fd);
			ft_write_error("pipe", NULL, strerror(errno));
			exit(EXIT_FAILURE);
		}
		close(fd);
	}
}

void	redirect_outfile_pipe(t_env_info *env, int fd[2],
		enum e_meta_character meta_next)
{
	if (meta_next == e_pipe)
	{
		if (dup2(fd[1], STDOUT_FILENO) == -1)
		{
			free_env(env);
			g_error = 1;
			close(fd[0]);
			close(fd[1]);
			ft_write_error("pipe", NULL, strerror(errno));
			exit(EXIT_FAILURE);
		}
		close(fd[0]);
		close(fd[1]);
	}
}
