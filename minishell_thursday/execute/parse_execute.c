/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_execute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 03:10:43 by thelmy            #+#    #+#             */
/*   Updated: 2024/09/26 03:51:15 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	panic(char *s)
{
	printf("%s\n", s);
	exit(1);
}

int	fork1(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		panic("fork");
	return (pid);
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

int	get_token_extend(char **s, char *es, char *whitespace, char *symbols)
{
	while ((*s) < es && ft_strchr(whitespace, *(*s)))
		(*s)++;
	if ((*s)[0] == '\"')
	{
		(*s)++;
		while ((*s) < es && (*s)[0] != '\"')
			(*s)++;
		(*s)++;
	}
	else if ((*s)[0] == '\'')
	{
		(*s)++;
		while ((*s) < es && (*s)[0] != '\'')
			(*s)++;
		(*s)++;
	}
	else
	{
		while ((*s) < es && (*s)[0] != '\"' && (*s)[0] != '\''
			&& !ft_strchr(whitespace, *(*s)) && !ft_strchr(symbols, *(*s)))
			(*s)++;
	}
	return ('a');
}

int	get_token_extend_red(char **s, char *es, char *whitespace, char *symbols)
{
	int	ret;

	ret = *(*s);
	if (*(*s) != '<' && *(*s) != '>')
		return (get_token_extend(s, es, whitespace, symbols));
	if (*(*s) == '<')
	{
		(*s)++;
		if (*(*s) == '<')
		{
			ret = 'h';
			(*s)++;
		}
	}
	else if (*(*s) == '>')
	{
		(*s)++;
		if (*(*s) == '>')
		{
			ret = '+';
			(*s)++;
		}
	}
	return (ret);
}

int	gettoken(char **ps, char *es, char **q, char **eq)
{
	char	*s;
	int		ret;
	char	whitespace[6];
	char	symbols[4];

	ft_strcpy(whitespace, " \t\r\n\v");
	ft_strcpy(symbols, "<|>");
	s = *ps;
	while (s < es && ft_strchr(whitespace, *s))
		s++;
	if (q)
		*q = s;
	ret = *s;
	if (*s == 0)
		return (ret);
	else if (*s == '|')
		s++;
	else
		ret = get_token_extend_red(&s, es, whitespace, symbols);
	if (eq)
		*eq = s;
	while (s < es && ft_strchr(whitespace, *s))
		s++;
	*ps = s;
	return (ret);
}

int	peek(char **ps, char *es, char *toks)
{
	char	*s;
	char	whitespace[6];

	ft_strcpy(whitespace, " \t\r\n\v");
	s = *ps;
	while (s < es && ft_strchr(whitespace, *s))
		s++;
	*ps = s;
	return (*s && ft_strchr(toks, *s));
}

t_cmd*	parseredirs(t_cmd *cmd, char **ps, char *es, t_heredoc **heredoc, int *last_exit_status)
{
	int		tok;
	char	*q;
	char	*eq;

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
		if (tok == '<')
			cmd = redircmd(cmd, q, eq, O_RDONLY, 0);
		else if (tok == '>')
			cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_TRUNC, 1);
		else if (tok == '+')
			cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_APPEND, 1);
		else if (tok == 'h')
			redircmd_h(q, eq, heredoc);
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

	ret = execcmd();
	if (!ret)
	{
		perror("Error creating exec command");
		*last_exit_status = 1;
		return (NULL);
	}
	cmd = (t_execcmd *)ret;
	argc = 0;
	ret = parseredirs(ret, ps, es, heredoc, last_exit_status);
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
		ret = parseredirs(ret, ps, es, heredoc, last_exit_status);
	}
	if (ret && cmd && cmd->argv[argc])
		cmd->argv[argc] = 0;
	if (ret && cmd && cmd->eargv[argc])
		cmd->eargv[argc] = 0;
	return (ret);
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

t_cmd* nulterminate(t_cmd *cmd, t_env *envir, int *last_exit_status)
{
	int			i;
	t_execcmd	*ecmd;
	t_pipecmd	*pcmd;
	t_redircmd	*rcmd;

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
	else if (cmd->type == REDIR)
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
	return (cmd);
}

t_cmd* remove_quotes(t_cmd *cmd)
{
  int i;
  t_execcmd *ecmd;
  t_pipecmd *pcmd;
  t_redircmd *rcmd;
  char *tmp;

  if (cmd == 0)
    return 0;
  if (cmd->type == EXEC) {
    ecmd = (t_execcmd*)cmd;
    i = 0;
    while (ecmd->argv[i]) {
      if (ecmd->argv[i][0] == '\"' && ecmd->argv[i][ft_strlen(ecmd->argv[i]) - 1] == '\"')
      {
        tmp = ecmd->argv[i];
        ecmd->argv[i] = ft_substr(tmp, 1, ft_strlen(tmp) - 2);
      }
      if (ecmd->argv[i][0] == '\'' && ecmd->argv[i][ft_strlen(ecmd->argv[i]) - 1] == '\'')
      {
        tmp = ecmd->argv[i];
        ecmd->argv[i] = ft_substr(tmp, 1, ft_strlen(tmp) - 2);
      }
      i++;
    }
  }
  else if (cmd->type == REDIR) {
    rcmd = (t_redircmd *)cmd;
    remove_quotes(rcmd->cmd);
  }
  else if (cmd->type == PIPE) {
    pcmd = (t_pipecmd *)cmd;
    remove_quotes(pcmd->left);
    remove_quotes(pcmd->right);
  }
  return cmd;
}

int check_quotes(char *str)
{
  int i = 0;
  int double_counter = 0;
  int single_counter = 0;

  while (str[i])
  {
    if (str[i] == '\"')
      double_counter++;
    if (str[i] == '\'')
      single_counter++;
    i++;
  }
  if (double_counter%2 != 0)
    return (1); //wrong input
  if (single_counter%2 != 0)
    return (1); //wrong input
  return (0); //
}

t_main parsecmd(char *s, t_env *envir, int *last_exit_status)
{
    char *es;
    struct cmd *cmd;
    t_main main; 
    main.heredoc = NULL;
    cmd = NULL;

    if (check_quotes(s)) //handling """ to avoid sigfault/invalid read
    {
      main.cmd = NULL;
      return (main);
    }
    es = s + ft_strlen(s);
    main.command = s;
    cmd = parsepipe(&s, es, &(main.heredoc), last_exit_status); //free tree, heredoc //free here if there is an error, otherwise, free in runcmd
    peek(&s, es, "");
    if (cmd && s != es) {
        write(2, "minishell: syntax error: unexpected token ", 42);
		write(2, s, ft_strlen(s));
    	*last_exit_status = 2;
    	freecmd(cmd, 1);
    	freeheredoc(main.heredoc);
		return main; //double check this
    }
    nulterminate(cmd, envir, last_exit_status); //free argv in the tree properly. Free argv first and then the nodes in the tree
    main.cmd = cmd;
    main.main_cmd = cmd;
    return (main);
}


// constructers
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

struct cmd* redircmd(struct cmd *subcmd, char *file, char *efile, int mode, int fd)
{
  struct redircmd *cmd;
  struct cmd *tmp;
  struct redircmd *tmp2;

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
  {
    tmp2 = (struct redircmd *)subcmd;
    while (tmp->type == REDIR)
    {
      tmp2 = (struct redircmd *)tmp;
      tmp = ((struct redircmd *)tmp)->cmd;
    }
    tmp2->cmd = (struct cmd*)cmd;
  }
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
  {
    // create a node, new head
    *heredoc = cmd;
  }
  else
  {
    // add to the linked list
    struct heredoc *tmp = *heredoc;
    while (tmp->next)
		tmp = tmp->next;
	tmp->next = cmd;
  }
}

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
