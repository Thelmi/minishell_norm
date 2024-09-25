/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:09:50 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/23 19:47:53 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	unset_exp(t_export **exp, char *variable)
{
	t_export	*tmp;
	t_export	*to_remove;

	if (!exp || !variable)
		return ;
	tmp = *exp;
	if (tmp != NULL && ft_strcmp(tmp->variable, variable) == true)
	{
		*exp = tmp->next;
		if (tmp)
			free_exp_node(tmp);
		tmp = NULL;
		return ;
	}
	while (tmp != NULL && tmp->next != NULL)
	{
		if (num_strncmp(tmp->next->variable, variable) == 0)
		{
			to_remove = tmp->next;
			tmp->next = tmp->next->next;
			free_exp_node(to_remove);
			to_remove = NULL;
			return ;
		}
		tmp = tmp->next;
	}
}

void	unset_env(t_env **env, char *variable)
{
	t_env	*tmp;
	t_env	*to_remove;

	if (!env || !variable)
		return ;
	tmp = *env;
	if (tmp != NULL && ft_strcmp(tmp->variable, variable) == true)
	{
		if (tmp->next != NULL)
			*env = tmp->next;
		else
		{
			*env = NULL;
			// if (env)
			// 	free(env);
			// env = NULL;
		}
		if (tmp)
			free_env_node(tmp);
		tmp = NULL;
		return ;
	}
	while (tmp != NULL && tmp->next != NULL)
	{
		if (num_strncmp(tmp->next->variable, variable) == 0)
		{
			to_remove = tmp->next;
			tmp->next = tmp->next->next;
			free_env_node(to_remove);
			to_remove = NULL;
			return ;
		}
		tmp = tmp->next;
	}
}

void	unset_export(t_export **exp, int ac, char **av, int *last_exit_status)
{
	int		i;
	char	*variable;

	i = 1;
	while (i < ac)
	{
		variable = av[i];
		if (variable == NULL || *variable == '\0')
		{
			write(2, "unset: Invalid argument: ", 25);
			write(2, av[i], ft_strlen(av[i]));
			write(2, "\n", 1);
			*last_exit_status = 1;
		}
		else
			unset_exp(exp, variable);
		i++;
	}
}

void	unset(t_env **env, int ac, char **av, int *last_exit_status)
{
	int		i;
	char	*variable;

	i = 1;
	while (i < ac)
	{
		variable = av[i];
		if (variable == NULL || *variable == '\0')
		{
			write(2, "unset: Invalid argument: ", 25);
			write(2, av[i], ft_strlen(av[i]));
			write(2, "\n", 1);
			*last_exit_status = 1;
		}
		else
			unset_env(env, variable);
		i++;
	}
}
