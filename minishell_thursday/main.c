/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:58:22 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/25 22:29:27 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	initialize_shell(char **ev, t_env **envir, t_export **exp,
		t_context *context)
{
	context->last_exit_status = 0;
	configure_terminal_behavior();
	*envir = storing_env(ev);
	*exp = storing_export(ev);
}

static void	command_loop(t_env **envir, t_export **exp, t_context *context)
{
	char		*command;
	static int	start;
	t_main		x;

	while (1)
	{
		setup_signals();
		command = readline("minishell$ ");
		start = 0;
		if (command == NULL)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (*command)
		{
			if (envir && *envir)
			{
				(*envir)->ev = convert_env(envir);
				signal(SIGQUIT, SIG_IGN);
				signal(SIGINT, SIG_IGN);
				fill_env(envir, (*envir)->ev);
				add_history(command);
				x = parsecmd(command, *envir, &context->last_exit_status);
				x.command = command;
				if (x.cmd)
					runcmd(x, envir, exp, &context->last_exit_status, start);
				if (x.cmd)
					freecmd(x.cmd, 0);
				if (envir && *envir && (*envir)->ev)
					free_double_pointer((*envir)->ev);
			}
			else
				printf("such a dangerous behavior, keep ur children safe\n");
		}
		free(command);
	}
	free_env(*envir);
	free_export(*exp);
}

int	main(int ac, char **av, char **ev)
{
	t_env		*envir;
	t_export	*exp;
	t_context	context;

	(void)ac;
	(void)av;
	initialize_shell(ev, &envir, &exp, &context);
	command_loop(&envir, &exp, &context);
	return (context.last_exit_status);
}
