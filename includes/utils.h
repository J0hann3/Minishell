/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:30:46 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/01 22:02:48 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "structs.h"

size_t	ft_strlen(const char *s);
void	*ft_calloc(size_t nmemb, size_t size);
char	*ft_strdup(const char *s);
void	free_str(char **str);
int		is_alpha(char c);
int		is_digit(char c);
int		ft_len(char **str);
int		ft_getenv(char **env, char *str);
int		ft_atouc(const char *str);
void	ft_write_error(const char *command,
			const char *argument, const char *message);
void	write_memory_exhausted(const char *context);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char *s1, char const *s2);
char	*ft_strjoin3(char *s1, char const *s2, char const *s3);
char	*ft_strnjoin(char *s1, char const *s2, size_t size);
int		ft_strcmp(const char *s1, const char *s2);
void	free_tree(t_ast **tree);
void	free_env(t_env_info *env);

#endif
