/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_with_args.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhelmy <mrhelmy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:03 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/28 14:59:40 by mrhelmy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int existing_args(char *av, t_export **export,
	int *last_exit_status, int *i)
{
	char	*variable;

	if (!is_valid_identifier(av))
	{
		write(2, "export: ", 8);
		write(2, av, ft_strlen(av));
		write(2, ": not a valid identifier\n", 25);
		*last_exit_status = 1;
		(*i) = (*i) + 1;
		return (1);
	}
	variable = ft_strdup(av);
	if (!variable)
	{
		perror("Error allocating memory for variable");
		*last_exit_status = 1;
		return (-1);
	}
	update_export(export, variable, NULL, last_exit_status);
	return (0);
}

// static int add_new_args()
// {
	
// }

static void invalid_identifier(char *av, char *variable,
	char *value, int *last_exit_status)
{
	write(2, "export: ", 8);
	write(2, av, ft_strlen(av));
	write(2, ": not a valid identifier\n", 25);
	free(variable);
	free(value);
	*last_exit_status = 1;
}

static int malloc_failed(char *variable, char *value, int *last_exit_status)
{
	perror("Error allocating memory for export variable");
	free(variable);
	free(value);
	*last_exit_status = 1;
	return (1);
}

static int new_args(char *av, t_env **env,
	t_export **export, int *last_exit_status)
{
	char	*variable;
	char	*value;
	char	*export_var;

	variable = substr_before_char(av, '=');
	value = substr_after_char(av, '=');
	if (variable == NULL || !*variable
		|| !is_valid_identifier(variable))
		invalid_identifier(av, variable, value, last_exit_status);
	else
	{
		update_env(env, variable, value, last_exit_status);
		if (variable)
			export_var = ft_strdup(variable);
		else
			export_var = NULL;
		if (!export_var && malloc_failed(variable, value, last_exit_status))
			return (0);
		update_export(export, export_var, ft_strdup(value), last_exit_status);
		value = NULL;
	}
	return (1);
}

void	export_with_args(t_norm x, t_export **export, int ac,
	int *last_exit_status)
{
	int		i;
	int check;
	char **av;
	t_env **env;

	i = 1;
	av = (char **)(x.var1);
	env = (t_env **)(x.var2);
	while (i < ac)
	{
		if (ft_strchr(av[i], '=') == NULL)
		{
			check = existing_args(av[i], export, last_exit_status, &i);
			if (check == -1)
				return ;
			if (check == 1)
				continue ;
		}
		else if (!new_args(av[i], env, export, last_exit_status))
			return ;
		i++;
	}
}
// void	export_with_args(t_norm x, t_export **export, int ac,
// 		int *last_exit_status)
// {
// 	int		i;
// 	char	*variable;
// 	char	*value;
// 	char	*export_var;
// 	char	*export_val;
// 	char	**av;
// 	t_env	**env;

// 	av = (char **)(x.var1);
// 	env = (t_env **)(x.var2);
// 	i = 1;
// 	while (i < ac)
// 	{
// 		if (ft_strchr(av[i], '=') == NULL)
// 		{
// 			if (!is_valid_identifier(av[i]))
// 			{
// 				write(2, "export: ", 8);
// 				write(2, av[i], ft_strlen(av[i]));
// 				write(2, ": not a valid identifier\n", 25);
// 				*last_exit_status = 1;
// 				i++;
// 				continue ;
// 			}
// 			variable = ft_strdup(av[i]);
// 			if (!variable)
// 			{
// 				perror("Error allocating memory for variable");
// 				*last_exit_status = 1;
// 				return ;
// 			}
// 			update_export(export, variable, NULL, last_exit_status);
// 		}
// 		else
// 		{
// 			variable = substr_before_char(av[i], '=');
// 			value = substr_after_char(av[i], '=');
// 			if (variable == NULL || !*variable
// 				|| !is_valid_identifier(variable))
// 			{
// 				write(2, "export: ", 8);
// 				write(2, av[i], ft_strlen(av[i]));
// 				write(2, ": not a valid identifier\n", 25);
// 				free(variable);
// 				free(value);
// 				*last_exit_status = 1;
// 			}
// 			else
// 			{
// 				update_env(env, variable, value, last_exit_status);
// 				if (variable)
// 					export_var = ft_strdup(variable);
// 				else
// 					export_var = NULL;
// 				if (!export_var)
// 				{
// 					perror("Error allocating memory for export variable");
// 					free(variable);
// 					free(value);
// 					*last_exit_status = 1;
// 					return ;
// 				}
// 				export_val = ft_strdup(value);
// 				update_export(export, export_var, export_val, last_exit_status);
// 				value = NULL;
// 			}
// 		}
// 		i++;
// 	}
// }
