/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_command.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krazikho <krazikho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:27:04 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/17 18:40:42 by krazikho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	is_builtin(char *command)
{
	if (ft_strcmp("echo", command) == true || ft_strcmp("cd", command) == true
		|| ft_strcmp("pwd", command) == true)
	{
		return (true);
	}
	else if (ft_strcmp("export", command) == true || ft_strcmp("unset",
			command) == true || ft_strcmp("env", command) == true)
	{
		return (true);
	}
	else if (ft_strcmp("exit", command) == true)
	{
		return (true);
	}
	else
	{
		return (false);
	}
}
