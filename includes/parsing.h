/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 19:46:19 by qthierry          #+#    #+#             */
/*   Updated: 2023/06/02 20:18:44 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include "structs.h"

typedef t_instruction			t_instr;
typedef enum e_meta_character	t_meta;

// ast_rec.c
t_ast		*create_sub_tree(char **input, t_ast *child, bool *has_error);

// ast_utils.c
t_ast		*create_node(const char *command);
t_meta		get_meta(char *input);

// ast.c
t_ast		*create_leaf(const char *input);
t_ast		*create_op_node(char *input);
t_ast		*create_tree(char *input, int *fd_heredocs, int len_fd);

// expand_dollars.c
t_char		*expand_dollars(char *input, size_t len,
				t_env_info *env_info, bool *is_ambigous);

// expand_utils.c
bool		is_expandable(const char *input);
size_t		get_size_of_var(const char *str);

// get_file_name.c
char		*get_file_name(t_char *input);
void		delete_file_name(t_char *input);

// open_files.c
int			read_fd(t_char *input);
int			open_fd(t_char *input);

// redirections.c
bool		open_all_fds(t_instruction *instruc, t_char *input, int fd_heredoc);

// remove_quotes.c
char		*remove_quotes(t_char *string);

// second_parsing.c
void		free_instructions(t_instr *instruc);
t_instr		*second_parsing(char *input,
				size_t command_size, t_env_info *env_info, int fd_heredocs);

#endif