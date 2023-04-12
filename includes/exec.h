/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:19:21 by jvigny            #+#    #+#             */
/*   Updated: 2023/04/11 22:07:52 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "structs.h"

int		ft_pipe(t_env_info *env, int fildes [2]);
int		exec(t_instruction *inst, t_env_info *env);
void	explore_tree(t_ast *tree, t_env_info *env, enum e_meta_character meta_before, int stat);

# endif
