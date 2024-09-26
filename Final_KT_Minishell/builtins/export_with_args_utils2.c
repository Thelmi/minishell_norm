/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_with_args_utils2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhelmy <mrhelmy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:10:03 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/26 22:54:23 by mrhelmy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int allocate_env_node(t_env	*tmp, int *last_exit_status)
{
	tmp->value = malloc(1);
	if (tmp->value == NULL)
	{
		perror("Error allocating memory for empty value");
		*last_exit_status = 1;
		return (0);
	}
	tmp->value[0] = '\0';
	return (1);
}

int available_env_args(t_env **env, char *variable, char *value,
		int *last_exit_status)
{
	t_env	*tmp;

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
				tmp->value = value;
			else
			if (!allocate_env_node(tmp, last_exit_status))
				return (1);
			return (1);
		}
		tmp = tmp->next;
	}
	return (0);
}

int allocate_value(char *variable, char *value, int *last_exit_status)
{
	if (value == NULL)
	{
		value = ft_strdup("");
		if (value == NULL)
		{
			perror("Error allocating memory for empty value");
			free(variable);
			*last_exit_status = 1;
			return (0);
		}
	}
	return (1);
}

void	update_env(t_env **env, char *variable, char *value,
		int *last_exit_status)
{
	t_env	*new_node;

	if (available_env_args(env, variable, value, last_exit_status))
		return ;
	if (!allocate_value(variable, value, last_exit_status))
		return ;
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
		return ;
	}
}
