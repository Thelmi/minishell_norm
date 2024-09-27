/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_with_args.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:18:03 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/27 13:56:03 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	export_with_args(t_norm x, t_export **export, int ac,
		int *last_exit_status)
{
	int		i;
	char	*variable;
	char	*value;
	char	*export_var;
	char	*export_val;
	char	**av;
	t_env	**env;

	av = (char **)(x.var1);
	env = (t_env **)(x.var2);
	i = 1;
	while (i < ac)
	{
		if (ft_strchr(av[i], '=') == NULL)
		{
			if (!is_valid_identifier(av[i]))
			{
				write(2, "export: ", 8);
				write(2, av[i], ft_strlen(av[i]));
				write(2, ": not a valid identifier\n", 25);
				*last_exit_status = 1;
				i++;
				continue ;
			}
			variable = ft_strdup(av[i]);
			if (!variable)
			{
				perror("Error allocating memory for variable");
				*last_exit_status = 1;
				return ;
			}
			update_export(export, variable, NULL, last_exit_status);
		}
		else
		{
			variable = substr_before_char(av[i], '=');
			value = substr_after_char(av[i], '=');
			if (variable == NULL || !*variable
				|| !is_valid_identifier(variable))
			{
				write(2, "export: ", 8);
				write(2, av[i], ft_strlen(av[i]));
				write(2, ": not a valid identifier\n", 25);
				free(variable);
				free(value);
				*last_exit_status = 1;
			}
			else
			{
				update_env(env, variable, value, last_exit_status);
				if (variable)
					export_var = ft_strdup(variable);
				else
					export_var = NULL;
				if (!export_var)
				{
					perror("Error allocating memory for export variable");
					free(variable);
					free(value);
					*last_exit_status = 1;
					return ;
				}
				export_val = ft_strdup(value);
				update_export(export, export_var, export_val, last_exit_status);
				value = NULL;
			}
		}
		i++;
	}
}
