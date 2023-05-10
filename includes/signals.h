/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 22:27:15 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/08 17:26:22 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "structs.h"

// signals.c
void	init_signals(struct sigaction act[2]);
void	new_line_signals(struct sigaction act[2]);
void	ign_signals(struct sigaction act[2]);
void	add_error_signals(struct sigaction act[2]);
void	none_interactive(struct sigaction act[2]);
void	reset_signals(struct sigaction act[2]);
void	heredocs_signal(struct sigaction act[2]);
void	heredocs_error_signal(struct sigaction act[2]);
void	crtl_c_interactive(int sig);		//for main process in interactive mode
void	error_new_line(int sig);	//for heredoc in child

#endif