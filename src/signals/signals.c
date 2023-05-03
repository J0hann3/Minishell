/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 22:01:04 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/03 15:45:56 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	crtl_c_interactive(int sig)		//for main process in interactive mode
{
	(void)sig;
	g_error = 130;
	write(STDERR_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	error_new_line(int sig)		//for heredoc in child
{
	(void)sig;
	write(STDERR_FILENO, "\n", 1);
	exit(sig + 128);
}

void	init_signals(struct sigaction act[2])
{	
	// CTRL-C
	act[0].sa_handler = crtl_c_interactive;
	sigemptyset(&(act[0]).sa_mask);
	act[0].sa_flags = 0;
	act[0].sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &act[0], NULL) == -1)
		ft_write_error("signal", NULL, strerror(errno));
	// CTRL-/
	act[1].sa_handler = SIG_IGN;
	sigemptyset(&act[1].sa_mask);
	act[1].sa_flags = 0;
	act[1].sa_flags = SA_RESTART;
	if (sigaction(SIGQUIT, &(act[1]), NULL) == -1)
		ft_write_error("signal", NULL, strerror(errno));
}


void	ign_signals(struct sigaction act[2])
{
	// CTRL-C
	act[0].sa_handler = SIG_IGN;
	if (sigaction(SIGINT, &act[0], NULL) == -1)
		ft_write_error("signal", NULL, strerror(errno));
	// CTRL-/
	act[1].sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &(act[1]), NULL) == -1)
		ft_write_error("signal", NULL, strerror(errno));
}

void	none_interactive(struct sigaction act[2])
{
	// CTRL-C
	act[0].sa_handler = SIG_DFL;
	if (sigaction(SIGINT, &act[0], NULL) == -1)
		ft_write_error("signal", NULL, strerror(errno));
	// CTRL-/
	act[1].sa_handler = SIG_DFL;
	if (sigaction(SIGQUIT, &(act[1]), NULL) == -1)
		ft_write_error("signal", NULL, strerror(errno));
}

void	reset_signals(struct sigaction act[2])
{	
	// CTRL-C
	act[0].sa_handler = crtl_c_interactive;
	if (sigaction(SIGINT, &act[0], NULL) == -1)
		ft_write_error("signal", NULL, strerror(errno));
	// CTRL-/
	act[1].sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &(act[1]), NULL) == -1)
		ft_write_error("signal", NULL, strerror(errno));
}

void	add_error_signals(struct sigaction act[2])
{
	// CTRL-C
	act[0].sa_handler = error_new_line;
	if (sigaction(SIGINT, &act[0], NULL) == -1)
		ft_write_error("signal", NULL, strerror(errno));
	// CTRL-/
	act[1].sa_handler = SIG_IGN;
	if (sigaction(SIGQUIT, &(act[1]), NULL) == -1)
		ft_write_error("signal", NULL, strerror(errno));
}

/**
 * In Interactive mode
 * ctrl-C displays a new prompt on a new line.	->none interactive = stop process and new prompt
 * ctrl-D exits the shell.		->work only in empty line
 * ctrl-\ does nothing		->none interactive = SIG_DFL
 * 
 * When Bash is interactive, in the absence of any traps, SIGINT is caught and handled 
 * (so that the wait builtin is interruptible). 
 * When Bash receives a SIGINT, it breaks out of any executing loops.
 * In all cases, Bash ignores SIGQUIT.
 */

// si built-in, alors main signaux = par defaut initialise

// interactif :
	// pendant rien : -> parent
		// - ctrl-C -> newline prompt
		// - ctrl-\ -> rien du tout
	// pendant commande : -> sur child uniquement
		// - ctrl-C -> stop (defaut) toute la ligne + newline
		// - ctrl-\ -> quit fonction la commande mais pas toute la ligne = SIG_DFL + newline
// non interactif : 
	// - ctrl-\ -> core dump la commande, mais continue le main
	// - ctrl-C -> fait exit sans ecrire exit, quitte le main