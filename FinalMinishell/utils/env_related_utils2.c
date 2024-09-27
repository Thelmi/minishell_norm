/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_related_utils2.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhelmy <mrhelmy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 22:13:09 by thelmy            #+#    #+#             */
/*   Updated: 2024/09/26 22:07:18 by mrhelmy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char    *concat_var_value(const char *variable, const char *value) {

	size_t var_len;
	size_t val_len;
	
	var_len = ft_strlen(variable);
	val_len = ft_strlen(value);
	char *result = malloc(var_len + val_len + 2);
	if (result == NULL) 
	{
		perror("malloc");
		return NULL;
	}
	ft_strlcpy(result, variable, var_len + 1);
	result[var_len] = '=';
	ft_strlcpy(result + var_len + 1, value, val_len + 1);
	return (result);
}

void fill_env(t_env **env, char **ev)
{
    t_env *tmp;

    tmp = *(env);
    while (tmp)
    {
		tmp->ev = ev;
		tmp = tmp->next;
    }
}

char **convert_env(t_env **env) 
{
    int count;
    t_env *temp;
	char **env_array;
	int j;
	
	count = 0;
	temp = *env;
    while (temp)
    {
        count++;
        temp = temp->next;
    }
    env_array = malloc((count + 1) * sizeof(char *));
    if (env_array == NULL)
        return (perror("malloc"), NULL);
    temp = *env;
    int i = 0;
    while (temp)
    {
        env_array[i] = concat_var_value(temp->variable, temp->value);
        if (env_array[i] == NULL) 
        {
            j = 0;
            while (j < i) {
                free(env_array[j]);
                j++;
            }
            free(env_array);
            return NULL;
        }
        i++;
        temp = temp->next;
    }
    env_array[count] = NULL;
    return env_array;
}
