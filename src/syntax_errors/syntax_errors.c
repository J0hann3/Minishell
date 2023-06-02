/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/10 19:48:35 by qthierry          #+#    #+#             */
/*   Updated: 2023/06/02 20:30:17 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

bool	has_closing_parenthesis(char **input,
			t_env_info *env, char *start_ptr);

/**
 * @brief Checks if the given quote is closed, forward input to the
 * closing quote, and write error if closing quote does not exist
 * 
 * @param input 
 * @return true 
 * @return false 
 */
bool	is_quote_closed(char **input)
{
	char		quote;
	const char	*error_quote = "unexpected EOF while looking for matching \
`\"'\nminishell: syntax error: unexpected end of file";

	quote = **input;
	(*input)++;
	while (**input)
	{
		if (**input == quote)
			return (true);
		(*input)++;
	}
	ft_write_error(NULL, NULL, (char *)error_quote);
	return (false);
}

bool	realloc_heredocs(t_env_info *env)
{
	env->len_heredocs++;
	env->fds_heredocs = ft_realloc(env->fds_heredocs, env->len_heredocs
			* sizeof(int), (env->len_heredocs + 1) * sizeof(int));
	if (!env->fds_heredocs)
		return (mem_exh("heredocs"), false);
	env->fds_heredocs[env->len_heredocs] = -1;
	return (true);
}

/**
 * @brief
 * return -1 on unexpected error
 * return 0 if syntax error and the write the error at the syntax error chars
 * return 1 if the syntax is ok
 * 
 * @param input 
 * @return int 
 */
static int	check_syntax_at(char **input, char *start_ptr, t_env_info *env)
{
	if (**input == '\'' || **input == '"')
		return (is_quote_closed(input));
	if (**input == '<' || **input == '>')
		return (is_redirection_ok(input, env));
	if (is_operator(*input))
		return (is_operator_ok(input, start_ptr) * 2);
	if (**input == '(')
		return (has_closing_parenthesis(input, env, start_ptr));
	if (**input == ')')
		return (ft_write_error(NULL, NULL,
				"syntax error near unexpected token `)'"), false);
	return (-1);
}

/**
 * @brief Check if the input is allow to be parse
 * 0 if the input is accepted
 * 1 if the input is empty
 * 2 if the input has a syntax error
 * 
 * @param input 
 * @return int 
 */
int	syntax_errors(char *input, t_env_info *env)
{
	char	*start_ptr;
	int		ret_val;

	env->len_heredocs = 0;
	env->fds_heredocs = malloc(1 * sizeof(int));
	if (!env->fds_heredocs)
		return (mem_exh("heredocs"), 2);
	env->fds_heredocs[env->len_heredocs] = -1;
	remove_multiple_wspaces(input);
	start_ptr = input;
	while (*input)
	{
		if (is_syntax_char(input))
		{
			ret_val = check_syntax_at(&input, start_ptr, env);
			if (ret_val < 1)
				return (128 * (env->fds_heredocs[0] == -2) + 2);
			else if (ret_val == 2 && !realloc_heredocs(env))
				return (2);
		}
		input++;
	}
	return (start_ptr == input);
}
