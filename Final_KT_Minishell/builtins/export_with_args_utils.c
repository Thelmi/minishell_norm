/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_with_args_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 14:38:17 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/27 13:53:11 by thelmy           ###   ########.fr       */
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

static void	add_export_node(t_export **export, t_export *new_node,
		char *variable, int *last_exit_status)
{
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
	add_export_node(export, new_node, variable, last_exit_status);
}
