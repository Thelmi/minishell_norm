/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 21:26:02 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/23 19:59:56 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	env_path(t_env *env, int *last_exit_status)
{
	t_env	*tmp;

	if (!env)
	{
		write(2, "Environment is empty or not initialized.\n", 41);
		*last_exit_status = 1;
		return (0);
	}
	tmp = env;
	while (tmp != NULL)
	{
		if (num_strncmp(tmp->variable, "PATH") == 0)
		{
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

void	env_func(t_env *env, int *last_exit_status)
{
	t_env	*tmp;

	if (!env)
	{
		write(2, "Environment is empty or not initialized.\n", 41);
		*last_exit_status = 1;
		return ;
	}
	tmp = env;
	while (tmp != NULL)
	{
		printf("%s", tmp->variable);
		printf("=");
		printf("%s\n", tmp->value);
		tmp = tmp->next;
	}
}
