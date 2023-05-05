/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 16:12:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/05 20:11:01 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief make redirection if fd != 0 + add g_error and write error ??
 * [ATTENTION]Don't have the name of arg_fd so I can't write the error
 * don't know what to do if something failed
 * 
 * @param inst 
 * @param env 
 */
void	redirection(t_instruction *inst)
{
	if (inst->infile >= 0)
	{
		// inst->s_infile = dup(STDIN_FILENO);
		// if (inst->s_infile == -1)
		// 	return (g_error = 1, ft_write_error("file", NULL, strerror(errno)));
		// if (dup2(inst->infile, STDIN_FILENO) == -1)
		// {
		// 	close(inst->s_infile);
		// 	inst->s_infile = -2;
		// 	g_error = 1;
		// 	ft_write_error(NULL, NULL, strerror(errno));
		// }
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
	// if (inst->s_infile >= 0)
	// {
	// 	if (dup2(inst->s_infile, STDIN_FILENO) == -1)
	// 	{
	// 		g_error = 1;
	// 		ft_write_error(NULL, NULL, strerror(errno));
	// 	}
	// 	close(inst->s_infile);
	// 	inst->s_infile = -2;
	// }
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
