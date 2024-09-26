#include "../minishell.h"

static char *remove_dir_quotes(t_main main)
{
  char *tmp;

  tmp = NULL;
  if ((main.rcmd)->file && (main.rcmd)->file[0] == '\"' && (main.rcmd)->file[ft_strlen((main.rcmd)->file) - 1] == '\"')
  {
    tmp = ft_substr((main.rcmd)->file, 1, ft_strlen((main.rcmd)->file) - 2);
    (main.rcmd)->file = tmp;
  } 
  if ((main.rcmd)->file && (main.rcmd)->file[0] == '\'' && (main.rcmd)->file[ft_strlen((main.rcmd)->file) - 1] == '\'')
  {
    tmp = ft_substr((main.rcmd)->file, 1, ft_strlen((main.rcmd)->file) - 2);
    (main.rcmd)->file = tmp;
  }
  return (tmp);
}

int open_dir(t_main main, int *last_exit_status)
{
  char *tmp;
  int fd;

  tmp = remove_dir_quotes(main);
  fd = open((main.rcmd)->file, (main.rcmd)->mode, 0644);
  if (tmp)
    free(tmp);
  if (fd < 0) {
    perror("open failed");
    *last_exit_status = 1;
    return (0);
  }
  if (dup2(fd, (main.rcmd)->fd) < 0) {
    perror("dup2 failed");
    close(fd);
    *last_exit_status = 1;
    return (0); //return ?
  }
  close(fd);
  return(1);
}

void redir(t_main main, t_env **envir, t_export **exp, int *last_exit_status)
{
    struct cmd *cmd = main.cmd;
  
    main.rcmd = (struct redircmd*)cmd;
    if ((main.rcmd)->file && !(((main.rcmd)->file[0] == '\"' || (main.rcmd)->file[0] == '\'') && (main.rcmd)->file[1] == '\0'))
    {
      if(!open_dir(main, last_exit_status))
        return ;
    }
    main.cmd = (main.rcmd)->cmd;
		runcmd(main, envir, exp, last_exit_status);
		dup2(main.saved_stdout, 1);
		dup2(main.saved_stdin, 0);
    close (main.saved_stdout);
    close (main.saved_stdin);
		return;
} 