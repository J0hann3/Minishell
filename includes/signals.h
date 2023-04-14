/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 22:27:15 by qthierry          #+#    #+#             */
/*   Updated: 2023/04/14 20:11:51 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

# include "structs.h"

// signals.c
void	init_signals(struct sigaction action, struct sigaction act_ign);
void	crtl_c_interactive(int sig);
// void	dfl_signal(struct sigaction action, struct sigation act_ign);
// void	activ_signal(struct sigaction action, struct sigation act_ign);


#endif