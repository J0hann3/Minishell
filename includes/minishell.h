/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 15:41:48 by jvigny            #+#    #+#             */
/*   Updated: 2023/03/10 15:59:49 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>

enum	e_meta_chararcter
{
	e_and,
	e_or,
	e_pipe
};

typedef union u_data
{
	struct s_instruction	*pdn_m;
	enum					meta;
}	t_data;

typedef struct s_instruction
{
	char	**command;
	char	**fd;
}	t_instruction;

typedef struct	s_node
{
	struct s_node	*child1;
	struct s_node	*child2;
	struct s_node	*parent;
	union u_data	data;
}	t_node;

void	echo(char **arg, char **env);
void	env(char **arg, char **env);
int		pwd(char **arg, char **env);

#endif