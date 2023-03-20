/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 19:41:23 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/20 19:36:43 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <stdint.h>
# include <errno.h>

typedef struct	s_env_info
{
	int		error;
	int		len_env;
	char	**env;
}	t_env_info;

// ----- Utils ------
size_t	ft_strlen(const char *s);
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strdup(const char *s);
void	free_str(char **str);
int		is_alpha(char c);
int		is_digit(char c);
int		is_variable_existing(char **env, char *str, int len_env);
int		ft_len(char **str);
int		ft_getenv(char **env, char *str);
int		ft_atouc(const char *str);
void	ft_write_error(char *command, char *argument, char *message);

// ------Init ------
t_env_info	*init_env(const char **env);
char	**init_arg(const char **arg);

// ------ Builtins -------
void	ft_echo(const char **arg, const t_env_info	*env);
void	ft_env(char **arg, t_env_info	*env);
t_env_info	*ft_export(char **arg, t_env_info	*env);
int		ft_pwd(char **arg, t_env_info	*env);
int		ft_unset(char **arg, t_env_info	*env);
int		ft_cd(char **arg, t_env_info	*env);
void	ft_exit(char **arg, t_env_info *env);

#endif