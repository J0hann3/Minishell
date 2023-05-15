/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/11 22:03:46 by qthierry          #+#    #+#             */
/*   Updated: 2023/05/15 23:29:58 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#define EE(x) free(x), x = NULL;

# include "exec.h"
# include "builtins.h"
# include "parsing.h"
# include "utils.h"
# include "signals.h"
# include "wildcard.h"

#endif