/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 15:41:48 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/22 16:16:16 by jvigny           ###   ########.fr       */
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
# include "builtins.h"

enum	e_meta_character
{
	e_and = 0,
	e_or,
	e_pipe,
	e_empty
};

typedef struct s_instruction
{
	char	**command;
	int		infile;
	int		outfile;
	int		outerror;
}	t_instruction;

typedef struct s_node
{

	struct s_node			*left;
	struct s_node			*right;
	struct s_node			*parent;
	char					*command;
	size_t					size;
	enum e_meta_character	meta;
}	t_node;


#endif