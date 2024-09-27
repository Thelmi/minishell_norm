/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhelmy <mrhelmy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:19:22 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/27 11:19:27 by mrhelmy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_main parsecmd(char *s, t_env *envir, int *last_exit_status)
{
    char *es;
    struct cmd *cmd;
    t_main main; 

    cmd = NULL;
    main.heredoc = NULL;
    main.cmd = NULL;
    if (check_quotes(s))
      return (main);
    es = s + ft_strlen(s);
    main.command = s;
    cmd = parsepipe(&s, es, &(main.heredoc), last_exit_status);
    peek(&s, es, "");
    if (cmd && s != es)
	{
		free_wrong_parsing(s, cmd, main, last_exit_status);
		return main;
    }
    nulterminate(cmd, envir, last_exit_status);
    main.cmd = cmd;
    main.main_cmd = cmd;
    return (main);
}

// t_cmd*	parseredirs(t_cmd *cmd, char **ps, char *es, t_heredoc **heredoc, int *last_exit_status)
t_cmd*	parseredirs(t_norm x, t_cmd *cmd, char *es, int *last_exit_status)
{
	int		tok;
	char	*q;
	char	*eq;
	t_norm	y;

	char **ps = (char **)(x.var1);
	t_heredoc **heredoc = (t_heredoc **)(x.var2);
	while (peek(ps, es, "<>"))
	{
		tok = gettoken(ps, es, 0, 0);
		if (gettoken(ps, es, &q, &eq) != 'a')
		{
			write(2, "bash: syntax error near unexpected token\n", 41);
			*last_exit_status = 2;
			freecmd(cmd, 1);
			cmd = NULL;
			return (NULL);
		}
		y.var1 = (void**)q;
		y.var2 = (void**)eq;
		if (tok == '<')
			cmd = redircmd(cmd, y, O_RDONLY, 0);
		else if (tok == '>')
			cmd = redircmd(cmd, y, O_WRONLY | O_CREAT | O_TRUNC, 1);
		else if (tok == '+')
			cmd = redircmd(cmd, y, O_WRONLY | O_CREAT | O_APPEND, 1);
		else if (tok == 'h')
			redircmd_h(q, eq, heredoc);
	}
	return (cmd);
}

t_cmd*	parsepipe(char **ps, char *es, t_heredoc **heredoc, int *last_exit_status)
{
	t_cmd	*cmd;

	cmd = parseexec(ps, es, heredoc, last_exit_status);
	if (cmd && peek(ps, es, "|"))
	{
		gettoken(ps, es, 0, 0);
		cmd = pipecmd(cmd, parsepipe(ps, es, heredoc, last_exit_status));
	}
	return (cmd);
}



t_cmd*	parseexec(char **ps, char *es, t_heredoc **heredoc, int *last_exit_status)
{
	char		*q;
	char		*eq;
	int			tok;
	int			argc;
	t_execcmd	*cmd;
	t_cmd		*ret;
	t_norm x;

	ret = execcmd();
	if (!ret)
	{
		perror("Error creating exec command");
		*last_exit_status = 1;
		return (NULL);
	}
	cmd = (t_execcmd *)ret;
	argc = 0;

	x.var1 = (void**)ps;
	x.var2 = (void**)heredoc;
	ret = parseredirs(x, ret, es, last_exit_status);
	// ret = parseredirs(ret, ps, es, heredoc, last_exit_status);
	if (!ret)
		return (NULL);
	while (ret && !peek(ps, es, "|"))
	{
		if ((tok = gettoken(ps, es, &q, &eq)) == 0)
			break ;
		if (tok != 'a')
		{
			*last_exit_status = 2;
			write(2, "bash: syntax error near unexpected token\n", 41);
			freecmd(ret, 1);
			return (NULL);
		}
		cmd->argv[argc] = q;
		cmd->eargv[argc] = eq;
		argc++;
		x.var1 = (void**)ps;
		x.var2 = (void**)heredoc;
		ret = parseredirs(x, ret, es, last_exit_status);
	}
	if (ret && cmd && cmd->argv[argc])
		cmd->argv[argc] = 0;
	if (ret && cmd && cmd->eargv[argc])
		cmd->eargv[argc] = 0;
	return (ret);
}

