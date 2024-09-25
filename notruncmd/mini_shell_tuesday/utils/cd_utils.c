/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krazikho <krazikho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 12:00:02 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/17 17:11:07 by krazikho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	update_env_for_cd(t_env **env, char *variable, char *value)
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
			tmp->value = ft_strdup(value);
			return ;
		}
		tmp = tmp->next;
	}
	new_node = create_env_nodes(variable, value);
	if (new_node != NULL)
	{
		new_node->next = *env;
		*env = new_node;
	}
	else
		printf("Error");
}
