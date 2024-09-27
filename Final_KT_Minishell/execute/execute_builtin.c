/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhelmy <mrhelmy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:43 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/27 11:18:46 by mrhelmy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	count_args(char **args)
{
	int	count;

	count = 0;
	while (args[count] != NULL)
		count++;
	return (count);
}

static void	execute_command(t_norm x, char echar[MAXARGS],
	int *last_exit_status, t_export **exp)
{
	int	count;
	t_norm y;

	char **args = (char **)(x.var1);
	t_env **envir = (t_env **)(x.var2);
	y.var1 = (void**)args;
	y.var2 = (void**)envir;
	count = count_args(args);
	if (ft_strcmp("echo", args[0]) == true)
		echo(args, echar);
	else if (ft_strcmp("cd", args[0]) == true)
		cd(args, envir, last_exit_status);
	else if (ft_strcmp("pwd", args[0]) == true)
		pwd(last_exit_status);
	else if (ft_strcmp("export", args[0]) == true)
	{
		if (args[1] != NULL)
			export_with_args(y, exp, count, last_exit_status);
		else
			export_no_arg(*exp, last_exit_status);
	}
}

t_env	*execute_builtin(t_norm x, char echar[MAXARGS],
	int *last_exit_status, t_export **exp)
{
	t_norm y;

	t_env **envir = (t_env **)(x.var1);
	char **args = (char **)(x.var2);
	if (!args || !args[0])
		return (*envir);
	*last_exit_status = 0;
	y.var1 = (void**)args;
	y.var2 = (void**)envir;
	execute_command(y, echar, last_exit_status, exp);
	// execute_command(args, envir, echar, last_exit_status, exp);
	if (ft_strcmp("unset", args[0]) == true)
	{
		if (args[1] != NULL)
		{
			unset(envir, count_args(args), args, last_exit_status);
			unset_export(exp, count_args(args), args, last_exit_status);
		}
	}
	else if (ft_strcmp("env", args[0]) == true)
	{
		env_func(*envir, last_exit_status);
	}
	else if (ft_strcmp("exit", args[0]) == true)
	{
		builtin_exit(args, last_exit_status);
	}
	return (*envir);
}
