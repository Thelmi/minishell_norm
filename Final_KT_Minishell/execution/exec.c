#include "../minishell.h"

void cat_with_heredoc(t_main main, t_env **envir, t_export **exp)
{
  write (1, main.input, ft_strlen(main.input));
  if (main.input)
    free (main.input);
  main.input = NULL;
  *(main.has_heredoc) = 0;

  free(main.command);//
  freecmd(main.cmd, 0);
  free_double_pointer((*envir)->ev);
  free_env(*envir);
  free_export(*exp);
  exit(0); // leaks?
}

void free_failed_exec(t_main main, t_env **envir, t_export **exp, int *last_exit_status)
{
  *last_exit_status = 127;
  if (main.input)
    free(main.input);
  main.input = NULL;
  *(main.has_heredoc) = 0;
  free(main.command);//
  freecmd(main.cmd, 0);
  free_double_pointer((*envir)->ev);
  free_env(*envir);
  free_export(*exp);
  perror("execve failed"); // change it...?
  exit(127);
}

void exec_exec(t_main main, t_env **envir, t_export **exp, int *last_exit_status)
{
  char *path;

  signal(SIGQUIT, SIG_DFL);
  signal(SIGINT, SIG_DFL); 
  if (ft_strcmp((main.ecmd)->argv[0], "cat") && (main.ecmd)->argv[1] == NULL && *(main.has_heredoc)) 
    cat_with_heredoc(main, envir, exp);
  remove_quotes(main.cmd);
  if (env_path(*envir, last_exit_status) || ft_strchr((main.ecmd)->argv[0], '/'))
  {
    if (ft_strcmp((main.ecmd)->argv[0], "cat"))
      *(main.stop_cat) = 0;
    if (execve((main.ecmd)->argv[0], (main.ecmd)->argv, (*envir)->ev) == -1)
    {
      path = find_path((main.ecmd)->argv[0], (*envir)->ev);
      if (path)
        execve(path, (main.ecmd)->argv, (*envir)->ev);
      free(path);
    }
  }
  free_failed_exec(main, envir, exp, last_exit_status);
}

void exec_signal_handler(t_main main, int *last_exit_status)
{
  int sig;

  if (WIFSIGNALED(main.status))
  {
    sig = WTERMSIG(main.status);
    if (sig == SIGINT)
    {
        write(1, "^C\n", 3);
        *last_exit_status = 130;
    }
    else if (sig == SIGQUIT)
    {
        write(1, "^\\Quit: 3\n", 10);
        *last_exit_status = 131;
    }
  }
  else
  {
      *last_exit_status = WEXITSTATUS(main.status);
  }
}

void reset_value(t_main main, int *last_exit_status)
{
  wait(&main.status);
  exec_signal_handler(main, last_exit_status);
  close (main.saved_stdout);
  close (main.saved_stdin);
  if (main.input)
  {
    free (main.input);
  }
  if (ft_strcmp((main.ecmd)->argv[0], "cat") && (main.ecmd)->argv[1] == NULL && *(main.has_heredoc)) //&& !(main.ecmd)->cat_flag 
  {
    *(main.has_heredoc) = 0;
  }
  if (!ft_strcmp((main.ecmd)->argv[0], "cat") || (ft_strcmp((main.ecmd)->argv[0], "cat") && (main.ecmd)->argv[1])) // Taha 5 is fixed here
  {
    *(main.stop_cat) = 0;
  }
  main.input = NULL;
}

int valid_args(t_main main, int *last_exit_status)
{
  if (!((main.ecmd)->argv[0]))
  {
    *last_exit_status = 1;
    if (main.input)
      free (main.input);
    main.input = NULL;
    return (0);
  }
  if (!ft_strcmp((main.ecmd)->argv[0], "cat") || (ft_strcmp((main.ecmd)->argv[0], "cat") && (main.ecmd)->argv[1]))
  {
    *(main.stop_cat) = 1;
  }
  return (1);
}

void exec(t_main main, t_env **envir, t_export **exp, int *last_exit_status)
{
  struct cmd *cmd = main.cmd;
  
  close (main.saved_stdout);
  close (main.saved_stdin);
  main.ecmd = (struct execcmd*)cmd;
  if (!valid_args(main, last_exit_status))
    return ;
  if (is_builtin((main.ecmd)->argv[0])==true) 
  {
    if (main.input)
      free (main.input);
    main.input = NULL;
    *envir = execute_builtin(envir , (main.ecmd)->argv, (main.ecmd)->echar, last_exit_status, exp); 
    return ;
  }
  if (fork() == 0)
    exec_exec(main, envir, exp, last_exit_status);
  else 
  {
    reset_value(main, last_exit_status);
    return ;
  }
} 


// if (!(main.ecmd)->argv[0])
// {
//   if (main.input)
//     free (main.input);
//   main.input = NULL;
//   return ;
// }
// if (!ft_strcmp((main.ecmd)->argv[0], "cat") || (ft_strcmp((main.ecmd)->argv[0], "cat") && (main.ecmd)->argv[1]))
// {
//   *(main.stop_cat) = 1;
// }
// if ((main.ecmd)->argv[0] == NULL)
// {
// 	*last_exit_status = 1;
//   if (main.input)
//     free(main.input);
//   main.input = NULL;
// 	return ; 
// }
