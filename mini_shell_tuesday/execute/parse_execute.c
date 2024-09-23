
#include "../minishell.h"


void panic(char *s)
{
  printf("%s\n", s);
  exit(1);
}
int fork1(void)
{
  int pid;

  pid = fork();
  if(pid == -1)
    panic("fork");
  return pid;
}

/* Function that will look for the path line inside the environment, will
 split and test each command path and then return the right one. */
char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == 0)
	{
    i++;
  }
  if (!envp)
    return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
		{
			free_arr(paths);
			return (path);
		}
		free(path);
		i++;
	}
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
	return (NULL);
}

void runcmd(t_main main, char **ev, t_env **envir, t_export **exp, int *last_exit_status)
{
	int p[2];
	struct pipecmd *pcmd;
  struct execcmd *ecmd;
	struct redircmd *rcmd;
  char *tmp;
	int status = 0;
  char *tmp3;
  char *tmp2;
  char *read = NULL;
  int i;
  char *tmp_input;
  char *tmp_input2;
  static char *input = 0;
  t_heredoc *tmp_h;
  static int cat_counter = 0;
  static int stop_cat = 0;
  static int has_heredoc = 0;
  int saved_stdin;
   int saved_stdout;

  struct cmd *cmd = main.cmd;
  if (cmd == NULL)
    return;

  saved_stdin = dup(STDIN_FILENO); //close all fds
	saved_stdout = dup(STDOUT_FILENO);
  
  while (main.heredoc)
  {
    i = 0;
    while (main.heredoc->argv[i] && main.heredoc->argv[i] != ' ')
    {
        i++;
    }
    tmp2 = ft_substr(main.heredoc->argv, 0, i);
    if (tmp2 && tmp2[0] == '\"' && tmp2[ft_strlen(tmp2) - 1] == '\"')
    {
      tmp3 = ft_substr(main.heredoc->argv, 1, ft_strlen(tmp2) - 2);
      free (tmp2);
      tmp2 = tmp3; // my bro
    } 
    if (tmp2 && tmp2[0] == '\'' && tmp2[ft_strlen(tmp2) - 1] == '\'')
    {
      tmp3 = ft_substr(main.heredoc->argv, 1, ft_strlen(tmp2) - 2);
      free (tmp2);
      tmp2 = tmp3; // my bro
    } 
    if (tmp2 == NULL) 
    {
          perror("Error allocating memory for heredoc delimiter");
          *last_exit_status = 1; // General error
          return ;
    }
      has_heredoc = 1;
    while (1)
    {
      write (1, "> ", 2);
      read = get_next_line(0);
      tmp_input = ft_substr(read, 0, ft_strlen(read) - 1);
      free (read);
      read = tmp_input;
      if (read == NULL)
        break ;
      if (num_strncmp(read, tmp2) == 0) {
          free(read);
          read = NULL;
          break;
      }
      if (main.heredoc->next == NULL)
      {
        tmp_input = ft_strjoin(read, "\n");
        if (!input)
          tmp_input2 = tmp_input;
        else
        {
          tmp_input2 = ft_strjoin(input, tmp_input);
          free(input);
          free(tmp_input);
        }
        input = tmp_input2;
      }
      free(read);
    }
    free(tmp2);
    tmp_h = main.heredoc;
    main.heredoc = main.heredoc->next;
    free(tmp_h);
    tmp_h = NULL;
  }
  freeheredoc(main.heredoc);
	if (cmd->type == EXEC) 
	{
    close (saved_stdout);
    close (saved_stdin);
		ecmd = (struct execcmd*)cmd;
    if (!ecmd->argv[0])
    {
      if (input)
        free (input);
      input = NULL;
      return ;
    }
    if (!ft_strcmp(ecmd->argv[0], "cat") || (ft_strcmp(ecmd->argv[0], "cat") && ecmd->argv[1]))
    {
      stop_cat = 1;
    }
		if (ecmd->argv[0] == NULL)
		{
			*last_exit_status = 1;
      if (input)
        free(input);
      input = NULL;
			return ; 
		}
    if (is_builtin(ecmd->argv[0])==true) //free as a builtin, only the execcmd
    {
      if (input)
      {
        free (input);
      }
      input = NULL;
      execute_builtin(envir , ecmd->argv, ecmd->echar, last_exit_status, exp);
      return ;
    }
    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, SIG_IGN);
    if (fork() == 0)
    {
      signal(SIGQUIT, SIG_DFL);
      signal(SIGINT, SIG_DFL); 
      if (ft_strcmp(ecmd->argv[0], "cat") && ecmd->argv[1] == NULL && has_heredoc) //&& !ecmd->cat_flag 
      {
        write (1, input, ft_strlen(input));
        if (input)
          free (input);
        input = NULL;
        has_heredoc = 0;

        free(main.command);//
        freecmd(main.cmd, 0);
        free_double_pointer((*envir)->ev);
        free_env(*envir);
        free_export(*exp);
        exit(0); // leaks?
      }
      remove_quotes(cmd);
      if (env_path(*envir, last_exit_status) || ft_strchr(ecmd->argv[0], '/'))
      {
        if (execve(ecmd->argv[0], ecmd->argv, (*envir)->ev) == -1)
        {
          char *path = find_path(ecmd->argv[0], (*envir)->ev);
          if (path)
              execve(path, ecmd->argv, (*envir)->ev);
            free(path);
        }
      }
      *last_exit_status = 127;
      if (input)
        free(input);
      input = NULL;
      has_heredoc = 0;
      free(main.command);//
      freecmd(main.cmd, 0);
      free_double_pointer((*envir)->ev);
      free_env(*envir);
      free_export(*exp);
      perror("execve failed"); // change it...?
      exit(127);
    }
    else 
    {
      wait(&status);
      if (WIFSIGNALED(status))
      {
          int sig = WTERMSIG(status);
          if (sig == SIGINT)
          {
              write(1, "^C\n", 3);  // Handle Ctrl+C in parent
              *last_exit_status = 130; // Exit status for Ctrl+C
          }
          else if (sig == SIGQUIT)
          {
              write(1, "^\\Quit: 3\n", 10);  // Handle Ctrl+\ in parent
              *last_exit_status = 131;  // Exit status for Ctrl+
          } 
      }
      else
      {
          *last_exit_status = 0;
      }
      close (saved_stdout);
      close (saved_stdin);
      if (input)
      {
        free (input);
      }
      if (ft_strcmp(ecmd->argv[0], "cat") && ecmd->argv[1] == NULL && has_heredoc) //&& !ecmd->cat_flag 
      {
        has_heredoc = 0;
      }
      input = NULL;
      return ;
    }
	} 
	else if (cmd->type == REDIR) 
	{
		rcmd = (struct redircmd*)cmd;
        tmp = NULL;
    if (rcmd->file && !((rcmd->file[0] == '\"' || rcmd->file[0] == '\'') && rcmd->file[1] == '\0'))
    {
      if (rcmd->file && rcmd->file[0] == '\"' && rcmd->file[ft_strlen(rcmd->file) - 1] == '\"')
        {
          tmp = ft_substr(rcmd->file, 1, ft_strlen(rcmd->file) - 2);
          rcmd->file = tmp; //we need to free the file name after using it
        } 
        if (rcmd->file && rcmd->file[0] == '\'' && rcmd->file[ft_strlen(rcmd->file) - 1] == '\'')
        {
          tmp = ft_substr(rcmd->file, 1, ft_strlen(rcmd->file) - 2);
          rcmd->file = tmp; //we need to free the file name after using it
        } 
      int fd = open(rcmd->file, rcmd->mode, 0644);
      if (tmp)
        free(tmp); //the same as free(rcmd->file);
      if (fd < 0) {
        perror("open failed");
        *last_exit_status = 1;
        return ;
      }
      if (dup2(fd, rcmd->fd) < 0) {
        perror("dup2 failed");
        close(fd);
        *last_exit_status = 1;
        return ;
      }
      close(fd);
    }
    main.cmd = rcmd->cmd;
		runcmd(main, ev, envir, exp, last_exit_status); // WARNING!!! make sure everything is free here, this is a recursive call WARNING!!!
		dup2(saved_stdout, 1);
		dup2(saved_stdin, 0);
    close (saved_stdout);
    close (saved_stdin);
    if (input)
    {
      free (input);
    }
		return;
	} 
	else if (cmd->type == PIPE) 
	{
		pcmd = (struct pipecmd*)cmd;
    if (input)
    {
      free (input);
    }
    input = NULL;
		if (pipe(p) < 0)
			panic("pipe failed");
		ecmd = (struct execcmd*)pcmd->left;
		if (ecmd->argv[0] == NULL)
		{
			printf("bash: syntax error near unexpected token `|\n");
			*last_exit_status = 2;
			return ;
		}
    if (ft_strcmp(ecmd->argv[0], "cat"))
          ecmd->cat_flag = 1;
    if (!stop_cat &&  ft_strcmp(ecmd->argv[0], "cat") && ecmd->cat_flag && !ecmd->argv[1])
    {
        cat_counter++;
    }
    int stopping_cat = 0;
    if (!(ft_strcmp(ecmd->argv[0], "cat") && !ecmd->argv[1]))
    {        
      if (fork1() == 0) 
      {
        close(p[0]);
        if (dup2(p[1], STDOUT_FILENO) < 0)
          panic("dup2 failed");
        close(p[1]);
        main.cmd = pcmd->left; //save the address correctly before modifying it for the next call. otherwise, you'll lose the pipe node and left/right orders and addressses
        close (saved_stdout);
        close (saved_stdin);
        runcmd(main, ev, envir, exp, last_exit_status);
        free(main.command);
        free_double_pointer((*envir)->ev);
        free_env(*envir);
        free_export(*exp);
        freecmd(main.main_cmd, 0);
        exit(*last_exit_status);
      }
    }
    else
    {
      stopping_cat = 1;
      if (pcmd->right->type != PIPE && ft_strcmp(((struct execcmd*)pcmd->right)->argv[0], "cat") && !(((struct execcmd*)pcmd->right)->argv[1]))
        cat_counter = 0;
			// {
    }
		wait(&status);
		*last_exit_status = WEXITSTATUS(status);
		if (fork1() == 0) 
		{
			close(p[1]);
      if (stopping_cat != 1)
      {
        if (dup2(p[0], STDIN_FILENO) < 0)
          panic("dup2 failed");
      }
			close(p[0]);
      main.cmd = pcmd->right;
      close (saved_stdout);
      close (saved_stdin);
      if (ft_strcmp(ecmd->argv[0], "cat"))
          ecmd->cat_flag = 0;
			runcmd(main, ev, envir, exp, last_exit_status); //WARNING!!! recursive call
      // free(tmp_main);
      free(main.command);
      free_double_pointer((*envir)->ev);
      free_env(*envir);
      free_export(*exp);
      freecmd(main.main_cmd, 0);
			exit(*last_exit_status);
		}
    if (pcmd->right->type == PIPE)
      cat_counter = 0;
		close(p[0]);
		close(p[1]);
		wait(&status);
    close (saved_stdout);
    close (saved_stdin);
	  *last_exit_status = WEXITSTATUS(status);
	}
 
  char *user_string;
  if (input)
    free(input);
  input = NULL;
  while (cat_counter)
  {
    user_string = get_next_line(0);
    free(user_string);
    cat_counter--;
  }
	return ;
}

// Parsing 
int gettoken(char **ps, char *es, char **q, char **eq) // add herdoc
{
  char *s;
  int ret;
	char whitespace[6];
	char symbols[4];

	ft_strcpy(whitespace, " \t\r\n\v");
	ft_strcpy(symbols, "<|>"); 
  s = *ps;
  while (s < es && ft_strchr(whitespace, *s))
      s++;
  if (q)
      *q = s;
  ret = *s;
  if (*s == 0) 
  {
      return ret;
  } 
    else if (*s == '|')
  {
      s++;
  }
  else if (*s == '<') 
  {
      s++;
      if (*s == '<') 
      {
          ret = 'h';
          s++;
      } 
	}
  else if (*s == '>') {
      s++;
      if (*s == '>') {
          ret = '+';
          s++;
      }
  } 
  else
  {
    ret = 'a';
    while(s < es && ft_strchr(whitespace, *s))
      s++;
    if (s[0] == '\"')
    {
      s++;
      while(s < es && s[0] != '\"')
        s++;
      s++;
    }
    else if (s[0] == '\'')
    {
      s++;
      while(s < es && s[0] != '\'')
        s++;
      s++;
    }
    else
    {
      while (s < es && s[0] != '\"' && s[0] != '\'' &&!ft_strchr(whitespace, *s) && !ft_strchr(symbols, *s))
        s++;
    }
  }
  if (eq)
      *eq = s;
  while (s < es && ft_strchr(whitespace, *s))
      s++;
  *ps = s;
  return ret;
}

int peek(char **ps, char *es, char *toks)
{
  char *s;

  char whitespace[6];
  ft_strcpy(whitespace, " \t\r\n\v");
  s = *ps;
  while(s < es && ft_strchr(whitespace, *s))
    s++;
  *ps = s;
  return (*s && ft_strchr(toks, *s));
}

struct cmd* parseredirs(struct cmd *cmd, char **ps, char *es, struct heredoc **heredoc, int *last_exit_status) // add herdoc, and struct
{
  int tok;
  char *q, *eq;
  while(peek(ps, es, "<>"))
  {
    tok = gettoken(ps, es, 0, 0);
    if(gettoken(ps, es, &q, &eq) != 'a')
    {
		write(2, "bash: syntax error near unexpected token\n", 41);
		*last_exit_status = 2;
		freecmd(cmd, 1); // Indicating a syntax error (like bash)
    cmd = NULL;
		return (NULL);
    }
    if(tok == '<'){
      cmd = redircmd(cmd, q, eq, O_RDONLY, 0);
    } else if(tok == '>') {
      cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_TRUNC, 1);
    } else if(tok == '+') {  // >>
      cmd = redircmd(cmd, q, eq, O_WRONLY | O_CREAT | O_APPEND, 1);
    } else if(tok == 'h') {  // <<
        redircmd_h(q, eq, heredoc);
    }
  }
  return cmd;
}

struct cmd* parseexec(char **ps, char *es, struct heredoc **heredoc, int *last_exit_status)
{
  char *q, *eq;
  int tok, argc;
  struct execcmd *cmd;
  struct cmd *ret;
  
  ret = execcmd();
  if (!ret) {
        perror("Error creating exec command");
        *last_exit_status = 1;
        return NULL;
    }
  cmd = (struct execcmd*)ret;
  argc = 0;
  ret = parseredirs(ret, ps, es, heredoc, last_exit_status); 
  if (!ret)
    return (NULL);
  while(ret && !peek(ps, es, "|")){ // I added ret
    if((tok=gettoken(ps, es, &q, &eq)) == 0)
      break;
    if (tok != 'a')
    {
		*last_exit_status = 2;
        write(2, "bash: syntax error near unexpected token\n", 41);
        freecmd(ret, 1);
        return NULL;
    }
    cmd->argv[argc] = q;
    cmd->eargv[argc] = eq;
    argc++;
    ret = parseredirs(ret, ps, es, heredoc, last_exit_status);
  }
  if (ret && cmd && cmd->argv[argc]) //&& cmd->argv
    cmd->argv[argc] = 0;
  if(ret && cmd && cmd->eargv[argc]) //&& cmd->eargv 
    cmd->eargv[argc] = 0;
  return ret;
}

struct cmd* parsepipe(char **ps, char *es, struct heredoc **heredoc, int *last_exit_status)
{
  struct cmd *cmd;
  
  cmd = parseexec(ps, es, heredoc, last_exit_status);
  if(cmd && peek(ps, es, "|")){ //I added cmd
    gettoken(ps, es, 0, 0);
    cmd = pipecmd(cmd, parsepipe(ps, es, heredoc, last_exit_status));
  }
  return cmd;
}

struct cmd* nulterminate(struct cmd *cmd, t_env *envir, int *last_exit_status)
{
  int i;
  struct execcmd *ecmd;
  struct pipecmd *pcmd;
  struct redircmd *rcmd;

  if (cmd == 0)
    return 0;
  if (cmd->type == EXEC) {
    ecmd = (struct execcmd*)cmd;
    i = 0;
    while (ecmd->argv[i]) {
      ecmd->argv[i] = ft_substr(ecmd->argv[i], 0, ecmd->eargv[i] - ecmd->argv[i]);
      ecmd->echar[i] = ecmd->eargv[i][0];
      if (ecmd->argv[i] == NULL) 
      {
        perror("ft_substr allocation failed");
        return NULL;
      }
      i++; 
    }
    modify_args(ecmd->argv, envir, last_exit_status);
  }
  else if (cmd->type == REDIR) {
    rcmd = (struct redircmd*)cmd;
    nulterminate(rcmd->cmd, envir, last_exit_status);
    *rcmd->efile = 0;
  }
  else if (cmd->type == PIPE)
  {
    pcmd = (struct pipecmd*)cmd;
    nulterminate(pcmd->left, envir, last_exit_status);
    nulterminate(pcmd->right, envir, last_exit_status);
  }
  return cmd;
}

struct cmd* remove_quotes(struct cmd *cmd)
{
  int i;
  struct execcmd *ecmd;
  struct pipecmd *pcmd;
  struct redircmd *rcmd;
  char *tmp;

  if (cmd == 0)
    return 0;
  if (cmd->type == EXEC) {
    ecmd = (struct execcmd*)cmd;
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
    rcmd = (struct redircmd*)cmd;
    remove_quotes(rcmd->cmd);
  }
  else if (cmd->type == PIPE) {
    pcmd = (struct pipecmd*)cmd;
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
