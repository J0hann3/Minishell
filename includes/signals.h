/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 22:27:15 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/27 16:52:29 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "structs.h"

// signals.c
void	init_signals(struct sigaction act[2]);
void	ign_signals(struct sigaction act[2]);
void	add_error_signals(struct sigaction act[2]);
void	none_interactive(struct sigaction act[2]);
void	reset_signals(struct sigaction act[2]);
void	crtl_c_interactive(int sig);
void	error_new_line(int sig);

#endif