/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 22:01:04 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/12 16:02:37 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void crtl_c_interactive(int sig)
{
	(void)sig;
	printf("coucou\n");
}

void	init_signals(void)
{
	struct sigaction action;
	
	sigemptyset(&action.sa_mask);
	action.sa_flags = 0;
	action.sa_handler = crtl_c_interactive;
	sigaction(SIGINT, &action, NULL);
}

/**
 * In Interactive mode
 * ctrl-C displays a new prompt on a new line.
 * ctrl-D exits the shell.
 * ctrl-\ does nothing
 */