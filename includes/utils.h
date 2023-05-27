/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:30:46 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/27 13:37:14 by jvigny           ###   ########.fr       */
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
void	mem_exh(const char *context);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char *s1, char const *s2);
char	*ft_strjoin3(char *s1, char const *s2, char const *s3);
char	*ft_strjoin_slash(char *s1, char *s2, int add_slash);
char	*ft_strnjoin(char *s1, char const *s2, size_t size);
int		ft_strcmp(const char *s1, const char *s2);
void	free_tree(t_ast **tree);
void	free_env(t_env_info *env);
void	close_fd_heredocs(t_env_info *env);
void	close_fd(t_instruction *inst);
char	*ft_strnstr(const char *big, const char *little, size_t len);
void	free_all(t_char **string);
char	*find_most_left_command(t_ast *tree);


// tchar_join.c
t_char	*tchar_njoin(t_char *s1, char const *s2, size_t size, bool is_inter);
t_char	*tchar_join(t_char *s1, const t_char *s2);

// tchar_ope.c
size_t	ft_tchar_len(const t_char *s);
t_char	*ft_tcharndup(const t_char *s, size_t n);
char	*ft_tchar_to_str(const t_char *input);
t_char	*ft_str_to_tchar(const char *str, bool is_inter);

// tchar_utils.c
size_t	skip_quotes_tchar(const t_char *input);
bool	inter_eq(t_char c1, char c2);
bool	tchar_is_operator(const t_char *c);

#endif
