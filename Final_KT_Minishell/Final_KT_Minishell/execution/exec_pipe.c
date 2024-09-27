/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:27:05 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/27 17:44:32 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	left_child(t_main main, t_env **envir, t_export **exp,
		int *last_exit_status)
{
	*(main.stop_cat_right_child) = 0;
	if (!(ft_strcmp((main.ecmd)->argv[0], "cat") && !(main.ecmd)->argv[1]))
	{
		if (fork1() == 0)
			run_free_left_child(main, envir, exp, last_exit_status);
	}
	else
	{
		*(main.stop_cat_right_child) = 1;
		if ((main.pcmd)->right->type
			&& ft_strcmp(((struct execcmd *)(main.pcmd)->right)->argv[0], "cat")
			&& !(((struct execcmd *)(main.pcmd)->right)->argv[1]))
			*(main.cat_counter) = 0;
	}
	return (*(main.stop_cat_right_child));
}

void	run_free_right_child(t_main main, t_env **envir, t_export **exp,
		int *last_exit_status)
{
	close(main.p[1]);
	if (*(main.stop_cat_right_child) != 1)
	{
		if (dup2(main.p[0], STDIN_FILENO) < 0)
			panic("dup2 failed");
	}
	close(main.p[0]);
	main.cmd = (main.pcmd)->right;
	close(main.saved_stdout);
	close(main.saved_stdin);
	if (ft_strcmp((main.ecmd)->argv[0], "cat"))
		(main.ecmd)->cat_flag = 0;
	runcmd(main, envir, exp, last_exit_status);
	free(main.command);
	free_double_pointer((*envir)->ev);
	free_env(*envir);
	free_export(*exp);
	freecmd(main.main_cmd, 0);
	exit(*last_exit_status);
}

int	right_child(t_main main, t_env **envir, t_export **exp,
		int *last_exit_status)
{
	if (((struct execcmd *)((main.pcmd)->right))->argv[0] != NULL)
	{
		if (fork1() == 0)
			run_free_right_child(main, envir, exp, last_exit_status);
	}
	else
	{
		printf("bash: syntax error near unexpected token `|\n");
		*last_exit_status = 2;
		close(main.p[0]);
		close(main.p[1]);
		close(main.saved_stdout);
		close(main.saved_stdin);
		return (0);
	}
	return (1);
}

int	run_pipe(t_main main, t_env **envir, t_export **exp, int *last_exit_status)
{
	if (ft_strcmp((main.ecmd)->argv[0], "cat"))
		(main.ecmd)->cat_flag = 1;
	if (!*(main.stop_cat) && ft_strcmp((main.ecmd)->argv[0], "cat")
		&& (main.ecmd)->cat_flag && !(main.ecmd)->argv[1])
		*(main.cat_counter) = *(main.cat_counter) + 1;
	*(main.stop_cat_right_child) = left_child(main, envir, exp,
			last_exit_status);
	wait(&(main.status));
	*last_exit_status = WEXITSTATUS(main.status);
	if (!right_child(main, envir, exp, last_exit_status))
		return (0);
	if ((main.pcmd)->right->type == PIPE)
		*(main.cat_counter) = 0;
	close(main.p[0]);
	close(main.p[1]);
	wait(&(main.status));
	*last_exit_status = WEXITSTATUS(main.status);
	close(main.saved_stdout);
	close(main.saved_stdin);
	return (1);
}

void	exec_pipe(t_main main, t_env **envir, t_export **exp,
		int *last_exit_status)
{
	struct cmd	*cmd;

	cmd = main.cmd;
	main.pcmd = (struct pipecmd *)cmd;
	if (main.input)
		free(main.input);
	main.input = NULL;
	(main.ecmd) = (struct execcmd *)(main.pcmd)->left;
	if ((main.ecmd)->argv[0] == NULL)
	{
		printf("bash: syntax error near unexpected token `|\n");
		close(main.saved_stdout);
		close(main.saved_stdin);
		*last_exit_status = 2;
		return ;
	}
	if (pipe(main.p) < 0)
		panic("pipe failed");
	if (!run_pipe(main, envir, exp, last_exit_status))
		return ;
}
