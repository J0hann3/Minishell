/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 19:41:23 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/01 21:42:23 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "structs.h"

// ------Init ------
t_env_info	*init_env(const char **env);
char		**init_arg(const char **arg);

// ------ Builtins -------
void		ft_echo(char **arg, t_env_info	*env);
void		ft_env(char **arg, t_env_info	*env);
void		ft_export(char **arg, t_env_info	*env);
void		ft_pwd(char **arg, t_env_info	*env);
void		ft_unset(char **arg, t_env_info	*env);
void		ft_cd(char **arg, t_env_info	*env);
void		ft_exit(t_instruction *arg, t_env_info *env);

char		*find_absolute_path(char *str);

#endif