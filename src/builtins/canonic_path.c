/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   canonic_path.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/03 16:01:28 by jvigny            #+#    #+#             */
/*   Updated: 2023/05/03 16:02:49 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	trim_slash(char *str, int i)
{
	int	letter_suppr;

	letter_suppr = 0;
	while (i >= 0)
	{
		if (str[i] == '/')
		{
			str[i] = '\0';
			letter_suppr++;
			i--;
		}
		else
			break ;
	}
	return (letter_suppr);
}

static int	delete_previous_dir(char *str, int index)
{
	int	letter_suppr;
	int	i;
	int	nb_slash;

	nb_slash = 0;
	letter_suppr = 0;
	i = index;
	while (index >= 0)
	{
		while (index > 0 && str[index] == '\0')
			index--;
		if (index > 0 && i != index && str[index] == '/')
			nb_slash++;
		if (nb_slash == 2 || index <= 0)
			break ;
		str[index] = '\0';
		letter_suppr++;
		index--;
	}
	return (letter_suppr);
}

static int	delete_dot_slash(char *str, int index)
{
	int	letter_suppr;
	int	i;

	letter_suppr = 0;
	i = index;
	while (index >= 0)
	{
		while (index > 0 && str[index] == '\0')
			index--;
		if (index <= 0 || (i != index && str[index] == '/'))
			break ;
		str[index] = '\0';
		letter_suppr++;
		index--;
	}
	return (letter_suppr);
}

int	canonical_form(char *str)
{
	int		i;
	int		letter_suppr;
	int		nb_dot;
	int		nb_slash;

	i = 0;
	letter_suppr = 0;
	while (str[i] != '\0')
	{
		nb_dot = 0;
		while (str[i] == '.')
		{
			++nb_dot;
			++i;
		}
		nb_slash = 0;
		while (str[i] != '\0' && str[i] == '/' && str[i + 1] == '/')
		{
			++nb_slash;
			++i;
		}
		if (nb_slash >= 1)
			letter_suppr += trim_slash(str, i - 1);
		if (str[i] == '/' || str[i] == '\0')
		{
			if (str[i] == '\0')
				i = i - 1;
			if (nb_dot == 2)
				letter_suppr += delete_previous_dir(str, i);
			else if (nb_dot == 1)
				letter_suppr += delete_dot_slash(str, i);
			if (str[i + 1] == '\0')
			{
				++i;
				break ;
			}
		}
		++i;
	}
	return (i - letter_suppr);
}
