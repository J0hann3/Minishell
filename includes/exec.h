/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:19:21 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/03 16:29:00 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "structs.h"

int		exec(t_instruction *inst, t_env_info *env);
void	explore_tree(t_ast *tree, t_env_info *env,
			enum e_meta_character meta_before);
void	multi_pipe(t_ast *tree, t_env_info *env, enum e_meta_character m_b,
			enum e_meta_character m_n);
void	redirection(t_instruction *inst);
void	redirection_fork(t_instruction *inst);
void	reset_redirection(t_instruction *inst);

char	*find_absolute_path(char *str);
char	*find_path_command(char *str, t_env_info *env);

#endif
