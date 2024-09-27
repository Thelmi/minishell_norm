/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:27:45 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/27 17:28:41 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	execute_cmd(t_main main, t_env **envir, t_export **exp,
		int *last_exit_status)
{
	struct cmd	*cmd;

	cmd = main.cmd;
	if (cmd == NULL)
		return (0);
	main.saved_stdin = dup(STDIN_FILENO);
	main.saved_stdout = dup(STDOUT_FILENO);
	main.input = heredoc_exec(main, last_exit_status, main.has_heredoc);
	if (cmd->type == EXEC)
		exec(main, envir, exp, last_exit_status);
	else if (cmd->type == REDIR)
	{
		redir(main, envir, exp, last_exit_status);
		if (main.input)
			free(main.input);
	}
	else if (cmd->type == PIPE)
	{
		exec_pipe(main, envir, exp, last_exit_status);
		*(main.cat_counter) = 0;
	}
	return (1);
}

void	cat_user_input(t_main main)
{
	char	*user_string;

	if (*(main.cat_counter) != 0)
		*(main.cat_counter) = *(main.cat_counter) - 1;
	while (*(main.cat_counter))
	{
		user_string = get_next_line(0);
		free(user_string);
		*(main.cat_counter) = *(main.cat_counter) - 1;
	}
}

void	runcmd(t_main main, t_env **envir, t_export **exp,
		int *last_exit_status)
{
	main.status = 0;
	if (*(main.start) == 0)
	{
		printf("initialize\n");
		*(main.stop_cat) = 0;
		*(main.cat_counter) = 0;
		*(main.start) = 1;
	}
	if (!execute_cmd(main, envir, exp, last_exit_status))
		return ;
	if (main.input)
		free(main.input);
	main.input = NULL;
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	cat_user_input(main);
	return ;
}
