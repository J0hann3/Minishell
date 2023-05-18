/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 22:01:04 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/19 00:40:52 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	init_signals(struct sigaction act[2])
{	
	act[0].sa_handler = crtl_c_interactive;
	sigemptyset(&(act[0]).sa_mask);
	act[0].sa_flags = 0;
	act[0].sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &act[0], NULL) == -1)
		ft_write_error("signal", NULL, strerror(errno));
	act[1].sa_handler = SIG_IGN;
	sigemptyset(&act[1].sa_mask);
	act[1].sa_flags = 0;
	act[1].sa_flags = SA_RESTART;
	if (sigaction(SIGQUIT, &(act[1]), NULL) == -1)
		ft_write_error("signal", NULL, strerror(errno));
}

void	ign_signals(struct sigaction act[2])
{
	act[0].sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &act[0], NULL) == -1)
		ft_write_error("signal", NULL, strerror(errno));
	act[1].sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &(act[1]), NULL) == -1)
		ft_write_error("signal", NULL, strerror(errno));
}

void	none_interactive(struct sigaction act[2])
{
	act[0].sa_handler = SIG_DFL;
	if (sigaction(SIGINT, &act[0], NULL) == -1)
		ft_write_error("signal", NULL, strerror(errno));
	act[1].sa_handler = SIG_DFL;
	if (sigaction(SIGQUIT, &(act[1]), NULL) == -1)
		ft_write_error("signal", NULL, strerror(errno));
}

void	reset_signals(struct sigaction act[2])
{	
	act[0].sa_handler = crtl_c_interactive;
	if (sigaction(SIGINT, &act[0], NULL) == -1)
		ft_write_error("signal", NULL, strerror(errno));
	act[1].sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &(act[1]), NULL) == -1)
		ft_write_error("signal", NULL, strerror(errno));
}

void	add_error_signals(struct sigaction act[2])
{
	act[0].sa_handler = error_new_line;
	if (sigaction(SIGINT, &act[0], NULL) == -1)
		ft_write_error("signal", NULL, strerror(errno));
	act[1].sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &(act[1]), NULL) == -1)
		ft_write_error("signal", NULL, strerror(errno));
}

// interactif :
	// pendant rien : -> parent
		// - ctrl-C -> newline prompt
		// - ctrl-\ -> rien du tout
	// pendant commande : -> sur child uniquement
		// - ctrl-C -> stop (defaut) toute la ligne + newline
		// - ctrl-\ -> quit fonction la commande mais
					// pas toute la ligne = SIG_DFL + newline
// non interactif : 
	// - ctrl-\ -> core dump la commande, mais continue le main
	// - ctrl-C -> fait exit sans ecrire exit, quitte le main