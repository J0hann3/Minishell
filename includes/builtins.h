/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 19:41:23 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/15 18:46:03 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTINS_H
# define BUILTINS_H

# include <stdint.h>

// ----- Utils ------
size_t	ft_strlen(const char *s);
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strdup(const char *s);
void	free_str(char **str);
int		is_alpha(char c);
int		is_digit(char c);
int		is_variable_existing(char **env, char *str, int len_env);
int		ft_len(char **str);

// ------Init ------
char	**init_env(const char **env);

// ------ Builtins -------
void	echo(const char **arg, const char **env);
void	env(char **arg, char **envp);
char	**export(char **arg, char **env);
int		pwd(char **arg, char **env);
int		unset(char **arg, char **env);

#endif