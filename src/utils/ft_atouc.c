/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atouc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/15 13:32:21 by jvigny            #+#    #+#             */
/*   Updated: 2023/06/02 18:21:43 by jvigny           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/**
 * @brief convert char * in unsigned char
 * 
 * @param str char *: text to convert in unsigned char
 * @return int: sucess -> return unsigned char,
 * else -> -1 if non_numeric value or not decimal
 */
int	ft_atouc(const char *str, bool *error)
{
	unsigned char	res;
	int				i;
	int				sign;

	res = 0;
	i = 0;
	sign = 1;
	*error = true;
	while (str[i] == ' ' || str[i] == '\f' || str[i] == '\r' || str[i] == '\n'
		|| str[i] == '\t' || str[i] == '\v')
		i++;
	if (str[i] == '-' || str[i] == '+')
		if (str[i++] == '-')
			sign = -1;
	while (str[i] >= '0' && str[i] <= '9')
	{
		res = res * 10 + str[i++] - '0';
		*error = false;
	}
	if (str[i] != '\0')
		*error = true;
	if (sign != 1)
		return ((unsigned char)res * sign);
	return (res);
}
