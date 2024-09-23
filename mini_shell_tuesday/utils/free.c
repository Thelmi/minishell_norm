/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:17:37 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/23 20:04:38 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void free_double_pointer(char **str)
{
    int i;
	
    i = 0;
    while (str && str[i])
    {
        free(str[i]);
        str[i] = NULL;
        i++;
    }
    if (str)
        free(str);
    str = NULL;
}

void free_env(t_env *env)
{
	t_env *tmp;
    
	tmp = env;
	while (tmp != NULL)
	{
		env = tmp;
		tmp = env->next;
		free(env->variable);
		free(env->value);
		free(env);
        env = NULL;
	}
}

void free_export(t_export *export)
{
	t_export *tmp;
	
	tmp = export;
	while (tmp != NULL)
	{
		tmp = export->next;
		free(export->variable);
		free(export->value);
		free(export);
		export = tmp;
	}
}

void free_exp_node(t_export *node)
{
    if (node)
    {
        free(node->variable);
        free(node->value);
        free(node);
    }
}

void free_env_node(t_env *node)
{
    if (node)
    {
        free(node->variable);
        free(node->value);
        node->ev = NULL;
        free(node);
    }
}

char	**free_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
	return (NULL);
}

void freecmd(struct cmd *cmd, int x)
{
    int i;
    struct execcmd *ecmd;
    struct pipecmd *pcmd;
    struct redircmd *rcmd;

    if (cmd == NULL)
        return;
    if (cmd->type == EXEC) {
        ecmd = (struct execcmd *)cmd;
        i = 0;
        while (x == 0 && ecmd->argv[i] != NULL) 
        {
            if (x == 0)
            {
                free(ecmd->argv[i]);
            }
            ecmd->argv[i] = NULL;
            i++;
        }
        if (cmd)
            free(cmd);
        cmd = NULL;
    } else if (cmd->type == PIPE) {
        pcmd = (struct pipecmd *)cmd;
        freecmd(pcmd->left, x);
        freecmd(pcmd->right, x);
        free(cmd);
        cmd = NULL;
    } else if (cmd->type == REDIR) {
        rcmd = (struct redircmd *)cmd;
        freecmd(rcmd->cmd, x);
        free(cmd);
        cmd = NULL;
    }
}

void freeheredoc(struct heredoc *heredoc)
{
    struct heredoc *tmp;
    while (heredoc) {
        tmp = heredoc;
        heredoc = heredoc->next;
        free(tmp);
        tmp = NULL;
    }
}

void cleanup(t_main *main)
{
    if (main->cmd) {
        freecmd(main->cmd, 0);
    }
    if (main->heredoc) {
        freeheredoc(main->heredoc);
    }
}
