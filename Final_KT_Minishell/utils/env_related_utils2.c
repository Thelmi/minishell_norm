/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_related_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 22:13:09 by thelmy            #+#    #+#             */
/*   Updated: 2024/09/27 15:42:11 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*concat_var_value(const char *variable, const char *value)
{
	size_t	var_len;
	size_t	val_len;
	char	*result;

	var_len = ft_strlen(variable);
	val_len = ft_strlen(value);
	result = malloc(var_len + val_len + 2);
	if (result == NULL)
	{
		perror("malloc");
		return (NULL);
	}
	ft_strlcpy(result, variable, var_len + 1);
	result[var_len] = '=';
	ft_strlcpy(result + var_len + 1, value, val_len + 1);
	return (result);
}

void	fill_env(t_env **env, char **ev)
{
	t_env	*tmp;

	tmp = *(env);
	while (tmp)
	{
		tmp->ev = ev;
		tmp = tmp->next;
	}
}

static int	count_env(t_env **env)
{
	int		count;
	t_env	*temp;

	count = 0;
	temp = *env;
	while (temp)
	{
		count++;
		temp = temp->next;
	}
	return (count);
}

static int	populate_env_array(char **env_array, t_env **env)
{
	t_env	*temp;
	int		i;

	temp = *env;
	i = 0;
	while (temp)
	{
		env_array[i] = concat_var_value(temp->variable, temp->value);
		if (env_array[i] == NULL)
		{
			return (i);
		}
		i++;
		temp = temp->next;
	}
	return (i);
}

char	**convert_env(t_env **env)
{
	int		count;
	char	**env_array;
	int		j;
	int		populated_count;

	count = count_env(env);
	env_array = malloc((count + 1) * sizeof(char *));
	if (env_array == NULL)
		return (perror("malloc"), NULL);
	populated_count = populate_env_array(env_array, env);
	while (populated_count < count)
	{
		if (populated_count < count)
		{
			j = 0;
			while (j < populated_count)
				free(env_array[j++]);
			return (free(env_array), NULL);
		}
	}
	env_array[count] = NULL;
	return (env_array);
}
