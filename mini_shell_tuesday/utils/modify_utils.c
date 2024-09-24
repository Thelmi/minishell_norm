/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 21:08:46 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/23 21:09:44 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	new_len(char *arg, t_env *envir, int *last_exit_status)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (arg[i])
	{
		if (arg[i] == '$' && arg[i + 1] == '?')
		{
			len += ft_strlen(ft_itoa(*last_exit_status));
			i += 2;
		}
		else if (arg[i] == '$' && ft_isalnum(arg[i + 1]))
		{
			len += handle_var_len(arg, &i, envir);
		}
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

	res = malloc(sizeof(char) * (new_len(arg, envir, last_exit_status) + 1));
	if (!res)
	{
		perror("malloc");
		return (NULL);
	}
	ft_memset(res, 0, new_len(arg, envir, last_exit_status) + 1);
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

int	handle_exit_status(char *res, int j, int *last_exit_status)
{
	(void)j;
	ft_strcat(res, ft_itoa(*last_exit_status));
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
