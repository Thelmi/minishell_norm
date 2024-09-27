/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_related_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/18 21:31:26 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/27 15:17:24 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*getcopyenv(char *str, t_env **envir)
{
	t_env	*tmp;

	tmp = *envir;
	while (tmp != NULL)
	{
		if (ft_strcmp(tmp->variable, str) == true)
		{
			return (tmp->value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

char	*substr_after_char(char *str, char c)
{
	int		i;
	int		len;
	char	*new_str;

	if (!str)
		return (NULL);
	i = 0;
	len = 0;
	while (str[i] && str[i] != c)
		i++;
	if (str[i] != c)
		return (NULL);
	i++;
	while (str[i + len])
		len++;
	new_str = malloc(sizeof(char) * (len + 1));
	if (!new_str)
		return (perror("malloc"), NULL);
	len = 0;
	while (str[i])
		new_str[len++] = str[i++];
	new_str[len] = '\0';
	return (new_str);
}

char	*substr_before_char(char *str, char c)
{
	int		i;
	char	*new_str;

	if (!str)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != c)
		i++;
	new_str = malloc(sizeof(char) * (i + 1));
	if (!new_str)
	{
		perror("malloc");
		return (NULL);
	}
	i = 0;
	while (str[i] && str[i] != c)
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[i] = '\0';
	return (new_str);
}

t_env	*create_env_nodes(char *variable_content, char *value_content)
{
	t_env	*list;

	list = malloc(sizeof(t_env));
	if (!list || (!variable_content && !value_content))
	{
		perror("malloc");
		return (NULL);
	}
	list->variable = variable_content;
	list->value = value_content;
	list->next = NULL;
	return (list);
}

t_env	*storing_env(char **ev)
{
	t_env	*env;
	t_env	*tmp;
	t_env	*newnode;
	int		i;

	if (!ev || !ev[0])
		return (NULL);
	env = create_env_nodes(substr_before_char(ev[0], '='),
			substr_after_char(ev[0], '='));
	if (!env)
		return (NULL);
	newnode = NULL;
	tmp = env;
	i = 1;
	while (ev[i])
	{
		newnode = create_env_nodes(substr_before_char(ev[i], '='),
				substr_after_char(ev[i], '='));
		if (!newnode)
			return (free_env(env), NULL);
		tmp->next = newnode;
		tmp = tmp->next;
		i++;
	}
	return (env);
}
