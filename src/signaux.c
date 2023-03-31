/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signaux.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 22:35:53 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/31 22:50:13 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <signal.h>

int	create_sigaction(void)
{
	struct sigaction	act;

	act.sa_sigaction = &receive;
	sigemptyset(&act.sa_mask);
	sigaction(SIGUSR1, &act, NULL);
}

/**
 * In Interactive mode
 * ctrl-C displays a new prompt on a new line.
 * ctrl-D exits the shell.
 * ctrl-\ does nothing
 */