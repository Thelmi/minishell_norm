/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: krazikho <krazikho@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 21:08:46 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/27 14:44:15 by krazikho         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	new_len(char *arg, t_env *envir, int *last_exit_status)
{
	int     len;
	int     i;
	char    *str;

	len = 0;
	i = 0;
	while (arg[i])
	{
		if (arg[i] == '$' && arg[i + 1] == '?')
		{
			str = ft_itoa(*last_exit_status);
			len += ft_strlen(str);
			free(str);
			i += 2;
		}
		else if (arg[i] == '$' && (ft_isalnum(arg[i + 1])|| arg[i + 1]=='_'))
			len += handle_var_len(arg, &i, envir);
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

char	*allocate_result(char *arg, t_env *envir, int *last_exit_status)
{
	char	*res;
	int     len;

	len = new_len(arg, envir, last_exit_status);
	res = malloc(sizeof(char) * (len + 1));
	if (!res)
	{
		perror("malloc");
		return (NULL);
	}
	ft_memset(res, 0, len + 1);
	return (res);
}

int	handle_var_len(char *arg, int *i, t_env *envir)
{
	int		var_len;
	char	var_name[256];

	var_len = 0;
	(*i)++;
	while (arg[*i] && (ft_isalnum(arg[*i]) || arg[*i] == '_'))
	{
		var_name[var_len++] = arg[*i];
		(*i)++;
	}
	var_name[var_len] = '\0';
	if (getcopyenv(var_name, &envir))
		return (ft_strlen(getcopyenv(var_name, &envir)));
	return (0);
}

int	handle_exit_status(char *res, int *last_exit_status)
{
	char    *str;

	str = ft_itoa(*last_exit_status);
	ft_strcat(res, str);
	free(str);
	return (ft_strlen(res));
}

int	handle_var_expansion(char *res, char *arg, int *i, t_env *envir)
{
	int		var_len;
	int		j;
	char	var_name[256];

	var_len = 0;
	j = ft_strlen(res);
	(*i)++;
	while (arg[*i] && (ft_isalnum(arg[*i]) || arg[*i] == '_'))
	{
		var_name[var_len++] = arg[(*i)++];
	}
	var_name[var_len] = '\0';
	if (getcopyenv(var_name, &envir))
	{
		ft_strcat(res, getcopyenv(var_name, &envir));
		j = ft_strlen(res);
	}
	return (j);
}
