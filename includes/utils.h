/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 14:30:46 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/02 20:31:26 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include "structs.h"

// char_utils.c
size_t		ft_strlen(const char *s);
char		*ft_strchr(const char *s, int c);
char		*ft_strndup(const char *s, size_t n);

// error.c
void		ft_write_error(const char *command,
				const char *argument, const char *message);
void		mem_exh(const char *context);

// free_tree.c
void		free_tree(t_ast **tree);

// free_utils.c
void		free_str(char **str);
void		free_env(t_env_info *env);
void		close_fd_heredocs(t_env_info *env);
void		close_fd(t_instruction *inst);

// ft_atouc.c
int			ft_atouc(const char *str);

// ft_calloc.c
void		*ft_calloc(size_t nmemb, size_t size);

// ft_itoa.c
char		*ft_itoa(int n);

// ft_split_quote.c
void		free_all(t_char **string);
t_char		**ft_split_quote(const t_char *string, int *nb_words);

// ft_split.c
char		**ft_split(char const *s, char c);

// ft_strcmp.c
int			ft_strcmp(const char *s1, const char *s2);

// ft_strndup.c
char		*ft_strdup(const char *s);

// ft_strjoin.c
char		*ft_strjoin(char *s1, char const *s2);
char		*ft_strjoin3(char *s1, char const *s2, char const *s3);
char		*ft_strnjoin(char *s1, char const *s2, size_t size);
char		*ft_strjoin_slash(char *s1, char *s2, int add_slash);

// ft_strnstr.c
char		*ft_strnstr(const char *big, const char *little, size_t len);

// init.c
t_env_info	*init_env(const char **env);

// other_utils.c
bool		eq(const char *s1, const char *s2);
bool		eqn(const char *s1, const char *s2, size_t n);
void		*ft_memcpy(void *dest, const void *src, size_t n);
void		*ft_realloc(void *ptr, size_t prev_size, size_t new_size);
void		remove_multiple_wspaces(char *input);

// skip_quote_par.c
size_t		skip_quotes(const char *input);
size_t		skip_parenthesis(const char *input);

// tchar_join.c
t_char		*tchar_njoin(t_char *s1,
				char const *s2, size_t size, bool is_inter);
t_char		*tchar_join(t_char *s1, const t_char *s2);

// tchar_ope.c
size_t		ft_tchar_len(const t_char *s);
t_char		*ft_tcharndup(const t_char *s, size_t n);
char		*ft_tchar_to_str(const t_char *input);
t_char		*ft_str_to_tchar(const char *str, bool is_inter);

// tchar_utils.c
size_t		skip_quotes_tchar(const t_char *input);
bool		inter_eq(t_char c1, char c2);
bool		tchar_is_operator(const t_char *c);

// utils.c
int			is_alpha(char c);
int			is_digit(char c);
int			ft_getenv(char **env, char *str);

#endif
