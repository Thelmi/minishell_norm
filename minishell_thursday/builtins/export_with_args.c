/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_with_args.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:10:03 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/26 17:30:58 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_valid_identifier(const char *str)
{
	int	i;

	i = 0;
	if (!str || str[i] == '\0')
		return (0);
	if (!(ft_isalpha(str[i]) || str[i] == '_'))
		return (0);
	i++;
	while (str[i] != '\0')
	{
		if (!(ft_isalnum(str[i]) || str[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

void	update_env(t_env **env, char *variable, char *value,
		int *last_exit_status)
{
	t_env	*tmp;
	t_env	*new_node;

	tmp = *env;
	while (tmp != NULL)
	{
		if (num_strncmp(tmp->variable, variable) == 0)
		{
			if (tmp->value)
				free(tmp->value);
			tmp->value = NULL;
			if (tmp->variable)
				free(tmp->variable);
			tmp->variable = variable;
			if (value != NULL)
			{
				tmp->value = value;
			}
			else
			{
				tmp->value = malloc(1);
				if (tmp->value == NULL)
				{
					perror("Error allocating memory for empty value");
					*last_exit_status = 1;
					return ;
				}
				tmp->value[0] = '\0';
			}
			return ;
		}
		tmp = tmp->next;
	}
	if (value == NULL)
	{
		value = ft_strdup("");
		if (value == NULL)
		{
			perror("Error allocating memory for empty value");
			free(variable);
			*last_exit_status = 1;
			return ;
		}
	}
	new_node = create_env_nodes(variable, value);
	new_node->ev = (*env)->ev;
	if (new_node != NULL)
	{
		new_node->next = *env;
		*env = new_node;
	}
	else
	{
		perror("Error allocating memory for new environment variable");
		free(variable);
		free(value);
		*last_exit_status = 1;
	}
}

void	update_export(t_export **export, char *variable, char *value,
		int *last_exit_status)
{
	t_export	*tmp;
	t_export	*new_node;

	tmp = *export;
	while (tmp != NULL)
	{
		if (num_strncmp(tmp->variable, variable) == 0)
		{
			free(variable);
			if (tmp->value)
				free(tmp->value);
			tmp->value = value;
			return ;
		}
		tmp = tmp->next;
	}
	new_node = create_export_nodes(variable, value);
	if (new_node != NULL)
	{
		new_node->next = *export;
		*export = new_node;
	}
	else
	{
		perror("Error allocating memory for new export variable");
		free(variable);
		*last_exit_status = 1;
	}
}

void	export_with_args(t_env **env, t_export **export, int ac, char **av,
		int *last_exit_status)
{
	int		i;
	char	*variable;
	char	*value;
	char	*export_var;
	char	*export_val;

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
