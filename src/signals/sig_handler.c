/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sig_handler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 16:23:06 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/19 00:39:46 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

//for main process in interactive mode
void	crtl_c_interactive(int sig)
{
	(void)sig;
	g_error = 130;
	write(STDERR_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

//for heredoc in child
void	error_new_line(int sig)
{
	(void)sig;
	write(STDERR_FILENO, "\n", 1);
	exit(sig + 128);
}
