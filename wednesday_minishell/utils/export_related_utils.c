/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_related_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krazikho <krazikho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 14:38:17 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/18 14:38:18 by krazikho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_export	*create_export_nodes(char *variable_content, char *value_content)
{
	t_export	*list;

	list = malloc(sizeof(t_export));
	if (!list)
	{
		perror("malloc");
		return (NULL);
	}
	list->variable = variable_content;
	list->value = value_content;
	list->next = NULL;
	return (list);
}

t_export	*storing_export(char **ev)
{
	t_export	*export;
	t_export	*tmp;
	t_export	*newnode;
	int			i;

	if (!ev || !ev[0])
		return (NULL);
	export = create_export_nodes(substr_before_char(ev[0], '='),
			substr_after_char(ev[0], '='));
	if (!export || !export->variable || !export->value)
	{
		free_export(export);
		return (NULL);
	}
	newnode = NULL;
	tmp = export;
	i = 1;
	while (ev[i])
	{
		newnode = create_export_nodes(substr_before_char(ev[i], '='),
				substr_after_char(ev[i], '='));
		if (!newnode)
			return (free_export(export), NULL);
		tmp->next = newnode;
		tmp = tmp->next;
		i++;
	}
	return (export);
}
