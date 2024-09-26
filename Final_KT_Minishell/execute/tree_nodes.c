/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_nodes.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhelmy <mrhelmy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 03:10:43 by thelmy            #+#    #+#             */
/*   Updated: 2024/09/26 21:45:50 by mrhelmy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

struct cmd* execcmd(void)
{
  struct execcmd *cmd;

  cmd = malloc(sizeof(*cmd));
  if (!cmd)
  {
    perror("malloc");
    return NULL;
  }
  ft_memset(cmd, 0, sizeof(*cmd));
  cmd->type = EXEC;
  return (struct cmd*)cmd;
}

struct cmd* pipecmd(struct cmd *left, struct cmd *right)
{
  struct pipecmd *cmd;

  cmd = malloc(sizeof(*cmd));
  if (!cmd)
  {
    perror("malloc");
    return NULL;
  }
  ft_memset(cmd, 0, sizeof(*cmd));
  cmd->type = PIPE;
  cmd->left = left;
  cmd->right = right;
  return (struct cmd*)cmd;
}

static struct cmd *sort_redir_nodes(struct cmd *subcmd, struct cmd *tmp, struct redircmd *cmd)
{
    struct redircmd *tmp2;

    tmp2 = (struct redircmd *)subcmd;
    while (tmp->type == REDIR)
    {
      tmp2 = (struct redircmd *)tmp;
      tmp = ((struct redircmd *)tmp)->cmd;
    }
    tmp2->cmd = (struct cmd*)cmd;
    return (tmp);
}
struct cmd* redircmd(struct cmd *subcmd, char *file, char *efile, int mode, int fd)
{
  struct redircmd *cmd;
  struct cmd *tmp;

  cmd = malloc(sizeof(*cmd));
  if (!cmd)
  {
    perror("malloc");
    return NULL;
  }
  ft_memset(cmd, 0, sizeof(*cmd));
  cmd->type = REDIR;
  tmp = subcmd;
  if (subcmd && subcmd->type == REDIR)
    tmp = sort_redir_nodes(subcmd, tmp, cmd);
  cmd->cmd = tmp;
  cmd->file = file;
  cmd->efile = efile;
  cmd->mode = mode;
  cmd->fd = fd;
  if (subcmd && subcmd->type == REDIR)
  {
    return (subcmd);
  }
  return (struct cmd*)cmd;
}
