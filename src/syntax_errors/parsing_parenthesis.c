/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_parenthesis.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/02 20:01:14 by qthierry          #+#    #+#             */
/*   Updated: 2023/06/02 20:05:33 by qthierry         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static const char	*get_par_error(char *par_start)
{
	size_t	i;

	par_start++;
	if (!*par_start)
		return ("newline");
	i = 0;
	while (par_start[i] && is_wspace(par_start[i]))
		i++;
	while (par_start[i] && !is_operator(par_start + i)
		&& !is_redirection(par_start[i])
		&& !is_wspace(par_start[i]) && !is_parenthesis(par_start[i]))
		i++;
	if (i == 0 && *par_start)
		i++;
	par_start[i] = 0;
	return (par_start);
}

static bool	is_parenthesis_left_ok(char *par_start, char *start_ptr)
{
	char	*tmp;
	char	*parenthesis;

	parenthesis = par_start;
	if (par_start == start_ptr)
		return (true);
	par_start--;
	while (par_start != start_ptr && is_wspace(*par_start))
		par_start--;
	if (*par_start == '(')
		return (true);
	tmp = par_start;
	while (par_start != start_ptr && *par_start == '|')
		par_start--;
	while (par_start != start_ptr && *par_start == '&')
		par_start--;
	if ((tmp - par_start == 2 && *(par_start + 1) == '&')
		|| (tmp - par_start == 1 && *(par_start + 1) == '|')
		|| (tmp - par_start == 2 && *(par_start + 1) == '|'))
		return (true);
	tmp = ft_strjoin3("syntax error near unexpected token `",
			get_par_error(parenthesis), "'");
	return (ft_write_error(NULL, NULL, tmp), free(tmp), false);
}

static bool	is_parenthesis_right_ok(char *str)
{
	size_t	i;
	int		tmp;
	char	*error;

	i = 1;
	while (is_wspace(str[i]))
		i++;
	if (str[i] == '\0' || str[i] == ')')
		return (true);
	if (str[i] == '(')
		return (ft_write_error(NULL, NULL,
				"syntax error near unexpected token `('"), false);
	tmp = i;
	while (str[i] && str[i] == '|')
		i++;
	while (str[i] && str[i] == '&')
		i++;
	if ((i - tmp == 2 && str[i - 1] == '&')
		|| (i - tmp == 1 && str[i - 1] == '|')
		|| (i - tmp == 2 && str[i - 1] == '|'))
		return (true);
	error = ft_strjoin3("syntax error near unexpected token `",
			get_par_error(str + 1), "'");
	return (ft_write_error(NULL, NULL, error), free(error), false);
}

static bool	check_on_caracter(char **input,
	t_env_info *env, char *par_start, char *start_ptr)
{
	if (**input == '('
		&& !has_closing_parenthesis(input, env, start_ptr))
		return (false);
	else if ((**input == '\'' || **input == '"')
		&& !is_quote_closed(input))
		return (false);
	else if ((**input == '<' || **input == '>')
		&& !is_redirection_ok(input, env))
		return (false);
	else if (is_operator(*input))
	{
		if (!is_operator_ok(input, par_start))
			return (false);
		else if (!realloc_heredocs(env))
			return (false);
	}
	(*input)++;
	return (true);
}

/**
 * @brief Checks if the given parenthesis is closed, forward input to the
 * closing parenthesis, and write error if closing parenthesis does not exist
 * Will check if the parenthesis has a left 
 * 
 * @param input 
 * @return true 
 * @return false 
 */
bool	has_closing_parenthesis(char **input, t_env_info *env, char *start_ptr)
{
	char		*par_start;

	par_start = (*input);
	(*input)++;
	while (is_wspace((**input)))
		(*input)++;
	if (**input == ')')
		return (ft_write_error(NULL, NULL,
				"syntax error near unexpected token `)'"), false);
	if (!is_parenthesis_left_ok(par_start, start_ptr))
		return (false);
	while (**input)
	{
		if (**input == ')')
			return (is_parenthesis_right_ok(*input));
		if (!check_on_caracter(input, env, par_start, start_ptr))
			return (false);
	}
	ft_write_error(NULL, NULL, "unexpected EOF while looking for matching \
`('\nminishell: syntax error: unexpected end of file");
	return (false);
}
