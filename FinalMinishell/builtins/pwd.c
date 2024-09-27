/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 14:14:05 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/25 23:02:07 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	pwd(int *last_exit_status)
{
	char	cwd[124];

	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("pwd");
		*last_exit_status = 1;
	}
	else
	{
		printf("%s\n", getcwd(cwd, sizeof(cwd)));
	}
}
