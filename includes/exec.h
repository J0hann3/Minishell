/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:19:21 by jvigny            #+#    #+#             */
/*   Updated: 2023/04/22 18:21:19 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "structs.h"

int		exec(t_instruction *inst, t_env_info *env, int has_ign_sig);
void	explore_tree(t_ast *tree, t_env_info *env, enum e_meta_character meta_before, int stat);

# endif
