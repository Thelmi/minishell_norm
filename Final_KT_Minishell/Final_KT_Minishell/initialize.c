/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialize.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 17:52:50 by thelmy            #+#    #+#             */
/*   Updated: 2024/09/27 17:56:39 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	initialize_shell(char **ev, t_env **envir, t_export **exp,
		t_context *context)
{
	context->last_exit_status = 0;
	configure_terminal_behavior();
	*envir = storing_env(ev);
	*exp = storing_export(ev);
}

t_main	initialize_main(t_main x)
{
	static int	start1;
	static int	start2;
	static int	start3;
	static int	start4;

	start1 = 0;
	start2 = 0;
	start3 = 0;
	start4 = 0;
	x.stop_cat = &start1;
	x.has_heredoc = &start2;
	x.executed_heredoc = &start3;
	x.stop_cat_right_child = &start4;
	return (x);
}
