/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execpipe_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 03:10:43 by thelmy            #+#    #+#             */
/*   Updated: 2024/09/27 17:47:59 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	run_free_left_child(t_main main, t_env **envir, t_export **exp,
		int *last_exit_status)
{
	close(main.p[0]);
	if (dup2(main.p[1], STDOUT_FILENO) < 0)
		panic("dup2 failed");
	close(main.p[1]);
	main.cmd = (main.pcmd)->left;
	close(main.saved_stdout);
	close(main.saved_stdin);
	runcmd(main, envir, exp, last_exit_status);
	free(main.command);
	free_double_pointer((*envir)->ev);
	free_env(*envir);
	free_export(*exp);
	freecmd(main.main_cmd, 0);
	exit(*last_exit_status);
}
