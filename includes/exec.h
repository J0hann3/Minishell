/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:19:21 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/24 18:06:42 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

# include "minishell.h"

enum	e_boolean
{
	e_true = 0,
	e_false
};

int		ft_pipe(t_env_info *env);
void	exec(t_instruction *inst, t_env_info *env);
// void	explore_tree(t_ast *tree, t_env_info *env, enum e_meta_character operand, int status)

# endif
