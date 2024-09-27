/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 17:03:47 by thelmy            #+#    #+#             */
/*   Updated: 2024/09/27 17:07:51 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	free_exec_cmd(struct execcmd *ecmd, int x)
{
	int	i;

	i = 0;
	if (!ecmd)
		return ;
	while (x == 0 && ecmd->argv[i] != NULL)
	{
		free(ecmd->argv[i]);
		ecmd->argv[i] = NULL;
		i++;
	}
	free(ecmd);
}

static void	free_pipe_cmd(struct pipecmd *pcmd, int x)
{
	if (!pcmd)
		return ;
	freecmd(pcmd->left, x);
	freecmd(pcmd->right, x);
	free(pcmd);
}

static void	free_redir_cmd(struct redircmd *rcmd, int x)
{
	if (!rcmd)
		return ;
	freecmd(rcmd->cmd, x);
	free(rcmd);
}

void	freecmd(struct cmd *cmd, int x)
{
	if (!cmd)
		return ;
	if (cmd->type == EXEC)
		free_exec_cmd((struct execcmd *)cmd, x);
	else if (cmd->type == PIPE)
		free_pipe_cmd((struct pipecmd *)cmd, x);
	else if (cmd->type == REDIR)
		free_redir_cmd((struct redircmd *)cmd, x);
}

// void freecmd(struct cmd *cmd, int x)
//{
//    int i;
//    struct execcmd *ecmd;
//    struct pipecmd *pcmd;
//    struct redircmd *rcmd;

//    if (cmd == NULL)
//        return ;
//    if (cmd->type == EXEC)
//    {
//        ecmd = (struct execcmd *)cmd;
//        i = 0;
//        while (x == 0 && ecmd->argv[i] != NULL)
//        {
//            if (x == 0)
//                free(ecmd->argv[i]);
//            ecmd->argv[i] = NULL;
//            i++;
//        }
//        if (cmd)
//            free(cmd);
//        cmd = NULL;
//    }
//    else if (cmd->type == PIPE)
//    {
//        pcmd = (struct pipecmd *)cmd;
//        freecmd(pcmd->left, x);
//        freecmd(pcmd->right, x);
//        free(cmd);
//        cmd = NULL;
//    }
//    else if (cmd->type == REDIR)
//    {
//        rcmd = (struct redircmd *)cmd;
//        freecmd(rcmd->cmd, x);
//        free(cmd);
//        cmd = NULL;
//    }

//}