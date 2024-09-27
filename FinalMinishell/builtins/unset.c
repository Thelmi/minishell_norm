/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhelmy <mrhelmy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:09:50 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/26 20:22:31 by mrhelmy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	unset(t_env **env, int ac, char **av, int *last_exit_status)
{
	int		i;
	char	*variable;

	i = 1;
	while (i < ac)
	{
		variable = av[i];
		if (variable == NULL || *variable == '\0')
		{
			write(2, "unset: Invalid argument: ", 25);
			write(2, av[i], ft_strlen(av[i]));
			write(2, "\n", 1);
			*last_exit_status = 1;
		}
		else
			unset_env(env, variable);
		i++;
	}
}
