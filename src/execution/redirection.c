/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 16:12:24 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/20 22:37:35 by qthierry         ###   ########.fr       */
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
