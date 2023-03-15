/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 01:08:36 by qthierry          #+#    #+#             */
/*   Updated: 2023/03/15 17:49:22 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.h"

char	*find_next_meta(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		if (is_meta_character(s[i]) && !(s[i] == '&' && s[i + 1] != '&'))
			return ((char *)(s + i));
		i++;
	}
	return (NULL);
}

void	print_size(char *deb, int size)
{
	printf("printing :\n");
	for (int i = 0; i < size; i++)
	{
		printf("%c\n", deb[i]);
	}
}

t_ast	*build_node(char *command, size_t size)
{
	t_ast	*node;

	node = ast_new_node(command);
	if (!node)
		return (NULL);
	node->size = size;
	return (node);
}

char	**parse_args(char *input)
{
	char	*cursor;
	char	*prev_cursor;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	cursor = input;
	prev_cursor = cursor;
	while (1)
	{
		cursor = find_next_meta(cursor);
		if (!cursor)
			break ;
		// print_size(prev_cursor, cursor - prev_cursor);
		cursor++;
		if (*cursor == *(cursor - 1))
			cursor++;
		prev_cursor = cursor;
	}
	return (NULL);
}

t_ast	*create_tree(char *input)
{
	//t_ast	*ast;
	//char	**args;

	//args = parse_args(input);
	//if (!args)
	//	return (NULL);
	
	

	return (NULL);
	//return (ast);
}
