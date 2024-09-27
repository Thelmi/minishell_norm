/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 03:10:43 by thelmy            #+#    #+#             */
/*   Updated: 2024/09/27 17:48:09 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	cat_with_heredoc(t_main main, t_env **envir, t_export **exp)
{
	write (1, main.input, ft_strlen(main.input));
	if (main.input)
		free (main.input);
	main.input = NULL;
	*(main.has_heredoc) = 0;
	free(main.command);
	freecmd(main.cmd, 0);
	free_double_pointer((*envir)->ev);
	free_env(*envir);
	free_export(*exp);
	exit(0);
}

void	free_failed_exec(t_main main, t_env **envir,
				t_export **exp, int *last_exit_status)
{
	*last_exit_status = 127;
	if (main.input)
		free(main.input);
	main.input = NULL;
	*(main.has_heredoc) = 0;
	free(main.command);
	freecmd(main.cmd, 0);
	free_double_pointer((*envir)->ev);
	free_env(*envir);
	free_export(*exp);
	perror("execve failed");
	exit(127);
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	if (!envp)
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	i = -1;
	while (paths[++i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
			return ((void)(free_arr(paths)), path);
		free(path);
	}
	i = -1;
	while (paths[++i])
		free(paths[i]);
	return (free(paths), NULL);
}

void	remove_exec_quotes(t_execcmd *ecmd)
{
	int		i;
	char	*tmp;

	i = 0;
	while (ecmd->argv[i])
	{
		if (ecmd->argv[i][0] == '\"' && ecmd->argv[i][ft_strlen(ecmd->argv[i])
			- 1] == '\"')
		{
			tmp = ecmd->argv[i];
			ecmd->argv[i] = ft_substr(tmp, 1, ft_strlen(tmp) - 2);
		}
		if (ecmd->argv[i][0] == '\'' && ecmd->argv[i][ft_strlen(ecmd->argv[i])
			- 1] == '\'')
		{
			tmp = ecmd->argv[i];
			ecmd->argv[i] = ft_substr(tmp, 1, ft_strlen(tmp) - 2);
		}
		i++;
	}
}

t_cmd	*remove_quotes(t_cmd *cmd)
{
	t_execcmd	*ecmd;
	t_pipecmd	*pcmd;
	t_redircmd	*rcmd;

	if (cmd == 0)
		return (0);
	if (cmd->type == EXEC)
	{
		ecmd = (t_execcmd *)cmd;
		remove_exec_quotes(ecmd);
	}
	else if (cmd->type == REDIR)
	{
		rcmd = (t_redircmd *)cmd;
		remove_quotes(rcmd->cmd);
	}
	else if (cmd->type == PIPE)
	{
		pcmd = (t_pipecmd *)cmd;
		remove_quotes(pcmd->left);
		remove_quotes(pcmd->right);
	}
	return (cmd);
}
