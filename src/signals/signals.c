/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 22:01:04 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/29 15:37:45 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	crtl_c_interactive(int sig)
{
	g_error = 128 + sig;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

void	error(int sig)
{
	write(1, "\n", 1);
	g_error = 128 + sig;
}

void	error_new_line(int sig)
{
	g_error = 128 + sig;
	// printf("eeeeeeeSIG : %d		error ; %d\n", sig, g_error);
	if (sig == SIGQUIT)
		write(1, "Quit (core dumped)", 18);			// reset redirection before print
	write(1, "\n", 1);
}

void	new_line(int sig)
{
	// printf("nnnnnnnSIG : %d		error ; %d\n", sig, g_error);
	if (sig == SIGQUIT)
		return ;
		// write(1, "Quit (core dumped)", 18);			// reset redirection before print
	write(1, "\n", 1);
}

void	init_signals(struct sigaction act[2])
{	
	// CTRL-C
	sigemptyset(&(act[0]).sa_mask);
	act[0].sa_flags = 0;
	act[0].sa_handler = crtl_c_interactive;
	sigaction(SIGINT, &act[0], NULL);
	// CTRL-/
	act[1].sa_handler = SIG_IGN;
	sigemptyset(&act[1].sa_mask);
	act[1].sa_flags = 0;
	sigaction(SIGQUIT, &(act[1]), NULL);
}

void	add_error_signals(struct sigaction act[2])
{
	// CTRL-C
	act[0].sa_handler = error_new_line;
	sigaction(SIGINT, &act[0], NULL);
	// CTRL-/
	act[1].sa_handler = error_new_line;
	sigaction(SIGQUIT, &(act[1]), NULL);
}

void	ign_signals(struct sigaction act[2])
{
	// CTRL-C
	act[0].sa_handler = SIG_IGN;
	sigaction(SIGINT, &act[0], NULL);
	// CTRL-/
	act[1].sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &(act[1]), NULL);
}

void	new_line_signals(struct sigaction act[2])
{
	// CTRL-C
	act[0].sa_handler = new_line;
	sigaction(SIGINT, &act[0], NULL);
	// CTRL-/
	act[1].sa_handler = new_line;
	sigaction(SIGQUIT, &(act[1]), NULL);
}

void	heredocs_signal(struct sigaction act[2])
{
	// CTRL-C
	act[0].sa_handler = SIG_DFL;
	sigaction(SIGINT, &act[0], NULL);
	// CTRL-/
	act[1].sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &(act[1]), NULL);
}

void	heredocs_error_signal(struct sigaction act[2])
{
	// CTRL-C
	act[0].sa_handler = new_line;
	sigaction(SIGINT, &act[0], NULL);
	// CTRL-/
	act[1].sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &(act[1]), NULL);
}

void	none_interactive(struct sigaction act[2])
{
	// CTRL-C
	act[0].sa_handler = SIG_DFL;
	sigaction(SIGINT, &act[0], NULL);
	// CTRL-/
	act[1].sa_handler = SIG_DFL;		//need to write Quit (core dumped)
	sigaction(SIGQUIT, &(act[1]), NULL);
}

void	reset_signals(struct sigaction act[2])
{	
	// CTRL-C
	act[0].sa_handler = crtl_c_interactive;
	sigaction(SIGINT, &act[0], NULL);
	// CTRL-/
	act[1].sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &(act[1]), NULL);
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