/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:19:21 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/02 15:08:08 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "structs.h"

int		exec(t_instruction *inst, t_env_info *env);
void	explore_tree(t_ast *tree, t_env_info *env, enum e_meta_character meta_before);
void	redirection(t_instruction *inst);
void	redirection_fork(t_instruction *inst);
void	reset_redirection(t_instruction *inst);

# endif
