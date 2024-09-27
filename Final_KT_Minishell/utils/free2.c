/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 17:49:22 by thelmy            #+#    #+#             */
/*   Updated: 2024/09/27 17:49:47 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_double_pointer(char **str)
{
	int	i;

	i = 0;
	while (str && str[i])
	{
		free(str[i]);
		str[i] = NULL;
		i++;
	}
	if (str)
		free(str);
	str = NULL;
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	tmp = env;
	while (tmp != NULL)
	{
		env = tmp;
		tmp = env->next;
		free(env->variable);
		free(env->value);
		free(env);
		env = NULL;
	}
}

void	free_export(t_export *export)
{
	t_export	*tmp;

	tmp = export;
	while (tmp != NULL)
	{
		tmp = export->next;
		free(export->variable);
		free(export->value);
		free(export);
		export = tmp;
	}
}
