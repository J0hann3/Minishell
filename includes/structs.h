/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 15:41:48 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/16 16:56:09 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <string.h>
# include <stdint.h>
# include <errno.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <signal.h>

extern int	g_error;

typedef struct s_char
{
	char	c;
	bool	is_inter;
}	t_char;

enum	e_meta_character
{
	e_empty = 0,
	e_or,
	e_pipe,
	e_and,
	e_empty_new
};

typedef struct s_env_info
{
	char				**env;
	int					len_env;
	int					*fds_heredocs;
	int					len_heredocs;
	struct sigaction	act[2];
	struct s_ast		*tree;
}	t_env_info;

typedef struct s_instruction
{
	char	**command;
	int		infile;
	int		outfile;
	int		outerror;
	int		s_infile;
	int		s_outfile;
	int		s_outerror;
}	t_instruction;

typedef struct s_ast
{
	struct s_ast			*left;
	struct s_ast			*right;
	struct s_ast			*parent;
	char					*command;
	int						fd_heredocs;
	size_t					size;
	enum e_meta_character	meta;
}	t_ast;

#endif