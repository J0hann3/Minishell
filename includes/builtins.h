/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 19:41:23 by jvigny            #+#    #+#             */
/*   Updated: 2023/04/12 16:16:31 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include "structs.h"

// ------Init ------
t_env_info	*init_env(const char **env);
char		**init_arg(const char **arg);

// ------ Builtins -------
void		ft_echo(char **arg, const t_env_info	*env);
void		ft_env(char **arg, t_env_info	*env);
t_env_info	*ft_export(char **arg, t_env_info	*env);
int			ft_pwd(char **arg, t_env_info	*env);
int			ft_unset(char **arg, t_env_info	*env);
int			ft_cd(char **arg, t_env_info	*env);
void		ft_exit(t_instruction *arg, t_env_info *env);

char		*find_absolute_path(char *str);

#endif