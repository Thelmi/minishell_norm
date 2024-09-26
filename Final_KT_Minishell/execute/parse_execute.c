/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_execute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhelmy <mrhelmy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 03:10:43 by thelmy            #+#    #+#             */
/*   Updated: 2024/09/26 20:58:33 by mrhelmy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void nulterminate_redir_pipe(t_cmd *cmd, t_env *envir, int *last_exit_status)
{
	t_pipecmd	*pcmd;
	t_redircmd	*rcmd;
	
	if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd *)cmd;
		nulterminate(rcmd->cmd, envir, last_exit_status);
		*rcmd->efile = 0;
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		nulterminate(pcmd->left, envir, last_exit_status);
		nulterminate(pcmd->right, envir, last_exit_status);
	}
}

t_cmd* nulterminate(t_cmd *cmd, t_env *envir, int *last_exit_status)
{
	t_execcmd	*ecmd;
	int			i;

	if (cmd == 0)
		return (0);
	if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		i = 0;
		while (ecmd->argv[i])
		{
			ecmd->argv[i] = ft_substr(ecmd->argv[i], 0,
					ecmd->eargv[i] - ecmd->argv[i]);
			ecmd->echar[i] = ecmd->eargv[i][0];
			if (ecmd->argv[i] == NULL)
				return (perror("ft_substr allocation failed"), NULL);
			i++;
		}
		modify_args(ecmd->argv, envir, last_exit_status);
	}
	else
		nulterminate_redir_pipe(cmd, envir, last_exit_status);
	return (cmd);
}


void redircmd_h(char *argv, char *eargv, struct heredoc **heredoc)
{
  struct heredoc *cmd;

  cmd = malloc(sizeof(*cmd));
  if (!cmd)
  {
    perror("malloc");
    return;
  }
  ft_memset(cmd, 0, sizeof(*cmd));
  cmd->type = HEREDOC;
  cmd->argv = argv;
  cmd->eargv = eargv;
  cmd->next = NULL;

  if (*heredoc == NULL)
  	*heredoc = cmd;
  else
  {
    struct heredoc *tmp = *heredoc;
    while (tmp->next)
		tmp = tmp->next;
	tmp->next = cmd;
  }
}

