/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 22:01:04 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/13 18:32:49 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	crtl_c_interactive(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	// need to change env->error to 130 = (128 + sig)
}

void	do_nothing(int sig)
{
	return ((void)sig);
}

void	init_signals(struct sigaction action, struct sigaction act_ign)
{	
	// CTRL-/
	act_ign.sa_handler = SIG_IGN;
	sigemptyset(&act_ign.sa_mask);
	act_ign.sa_flags = 0;
	sigaction(SIGQUIT, &act_ign, NULL);
	// CTRL-C
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	action.sa_handler = crtl_c_interactive;
	sigaction(SIGINT, &action, NULL);
}

/**
 * In Interactive mode
 * ctrl-C displays a new prompt on a new line.
 * ctrl-D exits the shell.		->work only in empty line
 * ctrl-\ does nothing
 */