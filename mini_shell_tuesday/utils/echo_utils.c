/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 14:44:13 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/23 22:54:14 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_only_n(const char *str)
{
	while (*str)
	{
		if (*str != 'n')
			return (false);
		str++;
	}
	return (true);
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	if (!s)
		return(0);
	while (s[i] != '\0')
		i++;
	return (i);
}
