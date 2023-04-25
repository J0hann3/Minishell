/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 15:41:48 by jvigny            #+#    #+#             */
/*   Updated: 2023/04/25 17:28:29 by qthierry         ###   ########.fr       */
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

enum	e_meta_character
{
	e_empty = 0,
	e_or,
	e_pipe,
	e_and,
	e_empty_new
};

typedef struct	s_env_info
{
	char				**env;
	int				len_env;
	struct sigaction	act[2];
	int				len_here;
	int				*fds_heredocs;
	struct s_ast	*tree;
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
	size_t					size;
	enum e_meta_character	meta;
}	t_ast;



#endif