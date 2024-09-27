/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/18 14:16:36 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/25 18:56:29 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	setup_signals()
{
	struct sigaction	sa_int;

	sa_int.sa_sigaction = sigint_handler;
	sa_int.sa_flags = SA_SIGINFO;
	sigemptyset(&sa_int.sa_mask);
	sigaction(SIGINT, &sa_int, NULL);

	signal(SIGQUIT, SIG_IGN);
	
}

void	configure_terminal_behavior()
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	sigint_handler(int sig, siginfo_t *info, void *context)
{
	t_context	*ctx;

	(void)sig;
	(void)info;
	ctx = (t_context *)context;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	ctx->last_exit_status = 1;
}