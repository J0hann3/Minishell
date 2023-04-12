/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 15:41:48 by jvigny            #+#    #+#             */
/*   Updated: 2023/04/12 15:54:17 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <string.h>
# include <stdint.h>
# include <errno.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include <signal.h>

enum	e_meta_character
{
	e_empty = 0,
	e_or,
	e_pipe,
	e_and
};

typedef struct	s_env_info
{
	int				error;
	int				len_env;
	char			**env;
	struct s_ast	*tree;
}	t_env_info;

typedef struct s_instruction
{
	char	**command;
	int		infile;
	int		outfile;
	int		outerror;
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