/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_builtin.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 21:26:02 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/23 22:27:39 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

long long	ft_atoi(const char *str)
{
	int			i;
	long long	sign;
	long long	result;

	i = 0;
	sign = 1;
	result = 0;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = (result * 10) + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

void	builtin_exit(char **args, int *last_exit_status)
{
	int		exit_status;
	char	*arg;

	exit_status = 0;
	arg = args[1];
	printf("exit\n");
	if (args[2] != NULL)
	{
		write(2, "bash: exit: too many arguments\n", 31);
		*last_exit_status = 1;
		return ;
	}
	if (arg != NULL)
	{
		while (*arg)
		{
			if (!ft_isdigit(*arg))
			{
				write(2, "exit: ", 6);
				write(2, args[1], ft_strlen(args[1]));
				write(2, ": numeric argument required\n", 28);
				*last_exit_status = 255;
				return ;
			}
			arg++;
		}
		exit_status = ft_atoi(args[1]);
		exit_status %= 256;
	}
	*last_exit_status = exit_status;
	exit(exit_status);
}
