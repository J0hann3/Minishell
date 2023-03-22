/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:19:21 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/22 16:15:52 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

// enum	e_builtins
// {
// 	e_echo = 1,
// 	e_cd,
// 	e_pwd,
// 	e_export,
// 	e_unset,
// 	e_env,
// 	e_exit
// };

void	exec(t_instruction *inst, t_env_info *env);

# endif
