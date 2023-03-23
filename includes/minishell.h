/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 15:41:48 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/23 00:19:19 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>

enum	e_meta_character
{
	e_empty = 0,
	e_pipe,
	e_or,
	e_and
};

typedef struct s_instruction
{
	char	**command;
	int		infile;
	int		outfile;
}	t_instruction;

typedef struct	s_ast
{
	struct s_ast			*left;
	struct s_ast			*right;
	struct s_ast			*parent;
	char					*command;
	size_t					size;
	enum e_meta_character	meta;
}	t_ast;

#endif