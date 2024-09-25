#include "../minishell.h"

void runcmd(t_main main, t_env **envir, t_export **exp, int *last_exit_status, int start)
{
	// int main.p[2];
	// struct pipecmd *pcmd;
  // struct execcmd *ecmd;
	// struct redircmd *rcmd;
  char *tmp;
	// int status = 0;
  static char *input = 0;
  static int cat_counter = 0;
  static int stop_cat = 0;
  static int has_heredoc = 0;
  // int saved_stdin;
  // int saved_stdout;
    
  main.status = 0;
  if (start == 0)
  {
    printf("initialize\n");
    stop_cat = 0;
      cat_counter = 0;
      start = 1;


  }
  struct cmd *cmd = main.cmd;
  if (cmd == NULL)
    return;

  main.saved_stdin = dup(STDIN_FILENO); 
  main.saved_stdout = dup(STDOUT_FILENO);
  
  input = heredoc_exec(main, last_exit_status, &has_heredoc);
  // while (main.heredoc)
  // {
  //   i = 0;
  //   while (main.heredoc->argv[i] && main.heredoc->argv[i] != ' ')
  //   {
  //       i++;
  //   }
  //   tmp2 = ft_substr(main.heredoc->argv, 0, i);
  //   if (tmp2 && tmp2[0] == '\"' && tmp2[ft_strlen(tmp2) - 1] == '\"')
  //   {
  //     tmp3 = ft_substr(main.heredoc->argv, 1, ft_strlen(tmp2) - 2);
  //     free (tmp2);
  //     tmp2 = tmp3; // my bro
  //   } 
  //   if (tmp2 && tmp2[0] == '\'' && tmp2[ft_strlen(tmp2) - 1] == '\'')
  //   {
  //     tmp3 = ft_substr(main.heredoc->argv, 1, ft_strlen(tmp2) - 2);
  //     free (tmp2);
  //     tmp2 = tmp3; // my bro
  //   } 
  //   if (tmp2 == NULL) 
  //   {
  //         perror("Error allocating memory for heredoc delimiter");
  //         *last_exit_status = 1; // General error
  //         return ;
  //   }
  //     has_heredoc = 1;
  //   while (1)
  //   {
  //     write (1, "> ", 2);
  //     read = get_next_line(0);
  //     tmp_input = ft_substr(read, 0, ft_strlen(read) - 1);
  //     free (read);
  //     read = tmp_input;
  //     if (read == NULL)
  //       break ;
  //     if (num_strncmp(read, tmp2) == 0) {
  //         free(read);
  //         read = NULL;
  //         break;
  //     }
  //     if (main.heredoc->next == NULL)
  //     {
  //       tmp_input = ft_strjoin(read, "\n");
  //       if (!input)
  //         tmp_input2 = tmp_input;
  //       else
  //       {
  //         tmp_input2 = ft_strjoin(input, tmp_input);
  //         free(input);
  //         free(tmp_input);
  //       }
  //       input = tmp_input2;
  //     }
  //     free(read);
  //   }
  //   free(tmp2);
  //   tmp_h = main.heredoc;
  //   main.heredoc = main.heredoc->next;
  //   free(tmp_h);
  //   tmp_h = NULL;
  // }
  // freeheredoc(main.heredoc);

	if (cmd->type == EXEC) 
	{
    close (main.saved_stdout);
    close (main.saved_stdin);
		main.ecmd = (struct execcmd*)cmd;
    if (!(main.ecmd)->argv[0])
    {
      if (input)
        free (input);
      input = NULL;
      return ;
    }
    if (!ft_strcmp((main.ecmd)->argv[0], "cat") || (ft_strcmp((main.ecmd)->argv[0], "cat") && (main.ecmd)->argv[1]))
    {
      stop_cat = 1;
    }
		if ((main.ecmd)->argv[0] == NULL)
		{
			*last_exit_status = 1;
      if (input)
        free(input);
      input = NULL;
			return ; 
		}
    if (is_builtin((main.ecmd)->argv[0])==true) 
    {
      if (input)
      {
        free (input);
      }
      input = NULL;
      *envir = execute_builtin(envir , (main.ecmd)->argv, (main.ecmd)->echar, last_exit_status, exp);
      // if (!*envir)
		  //     printf("nonnonoZnononononononononononono\n");
      // if (*envir)
      //   printf("nonnonoXnononononononononononono\n"); 
      return ;
    }
    if (fork() == 0)
    {
      signal(SIGQUIT, SIG_DFL);
      signal(SIGINT, SIG_DFL); 
      if (ft_strcmp((main.ecmd)->argv[0], "cat") && (main.ecmd)->argv[1] == NULL && has_heredoc) //&& !(main.ecmd)->cat_flag 
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
      if (env_path(*envir, last_exit_status) || ft_strchr((main.ecmd)->argv[0], '/'))
      {
        if (ft_strcmp((main.ecmd)->argv[0], "cat"))
        {
            stop_cat = 0;
        }
        if (execve((main.ecmd)->argv[0], (main.ecmd)->argv, (*envir)->ev) == -1)
        {
          char *path = find_path((main.ecmd)->argv[0], (*envir)->ev);
          if (path)
              execve(path, (main.ecmd)->argv, (*envir)->ev);
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
      wait(&main.status);
      if (WIFSIGNALED(main.status))
      {
          int sig = WTERMSIG(main.status);
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
      close (main.saved_stdout);
      close (main.saved_stdin);
      if (input)
      {
        free (input);
      }
      if (ft_strcmp((main.ecmd)->argv[0], "cat") && (main.ecmd)->argv[1] == NULL && has_heredoc) //&& !(main.ecmd)->cat_flag 
      {
        has_heredoc = 0;
      }
      if (!ft_strcmp((main.ecmd)->argv[0], "cat") || (ft_strcmp((main.ecmd)->argv[0], "cat") && (main.ecmd)->argv[1])) // Taha 5 is fixed here
      {
        stop_cat = 0;
      }
      input = NULL;
      return ;
    }
	} 
	else if (cmd->type == REDIR) 
	{
		main.rcmd = (struct redircmd*)cmd;
        tmp = NULL;
    if ((main.rcmd)->file && !(((main.rcmd)->file[0] == '\"' || (main.rcmd)->file[0] == '\'') && (main.rcmd)->file[1] == '\0'))
    {
      if ((main.rcmd)->file && (main.rcmd)->file[0] == '\"' && (main.rcmd)->file[ft_strlen((main.rcmd)->file) - 1] == '\"')
        {
          tmp = ft_substr((main.rcmd)->file, 1, ft_strlen((main.rcmd)->file) - 2);
          (main.rcmd)->file = tmp; //we need to free the file name after using it
        } 
        if ((main.rcmd)->file && (main.rcmd)->file[0] == '\'' && (main.rcmd)->file[ft_strlen((main.rcmd)->file) - 1] == '\'')
        {
          tmp = ft_substr((main.rcmd)->file, 1, ft_strlen((main.rcmd)->file) - 2);
          (main.rcmd)->file = tmp; //we need to free the file name after using it
        } 
      int fd = open((main.rcmd)->file, (main.rcmd)->mode, 0644);
      if (tmp)
        free(tmp); //the same as free((main.rcmd)->file);
      if (fd < 0) {
        perror("open failed");
        *last_exit_status = 1;
        return ;
      }
      if (dup2(fd, (main.rcmd)->fd) < 0) {
        perror("dup2 failed");
        close(fd);
        *last_exit_status = 1;
        return ;
      }
      close(fd);
    }
    main.cmd = (main.rcmd)->cmd;
		runcmd(main, envir, exp, last_exit_status, start); // WARNING!!! make sure everything is free here, this is a recursive call WARNING!!!
		dup2(main.saved_stdout, 1);
		dup2(main.saved_stdin, 0);
    close (main.saved_stdout);
    close (main.saved_stdin);
    if (input)
    {
      free (input);
    }
		return;
	} 
	else if (cmd->type == PIPE) 
	{
		main.pcmd = (struct pipecmd*)cmd;
    if (input)
    {
      free (input);
    }
    input = NULL;
		if (pipe(main.p) < 0)
			panic("pipe failed");
		(main.ecmd) = (struct execcmd*)(main.pcmd)->left;
		if ((main.ecmd)->argv[0] == NULL)
		{
			printf("bash: syntax error near unexpected token `|\n");
			*last_exit_status = 2;

			return ;
		}
    if (ft_strcmp((main.ecmd)->argv[0], "cat"))
          (main.ecmd)->cat_flag = 1;
    if (!stop_cat &&  ft_strcmp((main.ecmd)->argv[0], "cat") && (main.ecmd)->cat_flag && !(main.ecmd)->argv[1])
    {
        cat_counter++;
    }
    int stopping_cat = 0;
    if (!(ft_strcmp((main.ecmd)->argv[0], "cat") && !(main.ecmd)->argv[1]))
    {        
      if (fork1() == 0) 
      {
        close(main.p[0]);
        if (dup2(main.p[1], STDOUT_FILENO) < 0)
          panic("dup2 failed");
        close(main.p[1]);
        main.cmd = (main.pcmd)->left;
        close (main.saved_stdout);
        close (main.saved_stdin);
        runcmd(main, envir, exp, last_exit_status, start);
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
      if ((main.pcmd)->right->type && ft_strcmp(((struct execcmd*)(main.pcmd)->right)->argv[0], "cat") && !(((struct execcmd*)(main.pcmd)->right)->argv[1])) // Taha check this line
      {
        printf("Yes\n");
        cat_counter = 0;
      }
			// {
    }
		wait(&(main.status));
		*last_exit_status = WEXITSTATUS(main.status);
    if (((struct execcmd*)((main.pcmd)->right))->argv[0] != NULL) // Taha 5 is fixed here
		{
      // if ((main.pcmd)->right->type == PIPE)
      //   cat_counter = 0;
      if (fork1() == 0) 
      {
        close(main.p[1]);
        if (stopping_cat != 1)
        {
          if (dup2(main.p[0], STDIN_FILENO) < 0)
            panic("dup2 failed");
        }
        close(main.p[0]);
        main.cmd = (main.pcmd)->right;
        close (main.saved_stdout);
        close (main.saved_stdin);
        if (ft_strcmp((main.ecmd)->argv[0], "cat"))
            (main.ecmd)->cat_flag = 0;
        runcmd(main, envir, exp, last_exit_status, start); //WARNING!!! recursive call
        // free(tmp_main);
        free(main.command);
        free_double_pointer((*envir)->ev);
        free_env(*envir);
        free_export(*exp);
        freecmd(main.main_cmd, 0);
      exit(*last_exit_status);
      }
    }
    else // Taha I modifed this to take care of the empty right side of the pipe
    {
      printf("bash: syntax error near unexpected token `|\n");
			*last_exit_status = 2;
      close(main.p[0]);
		  close(main.p[1]);
      close (main.saved_stdout);
    	close (main.saved_stdin);
      return ;
    }
    if ((main.pcmd)->right->type == PIPE)
      cat_counter = 0;
		close(main.p[0]);
		close(main.p[1]);
		wait(&(main.status));
		*last_exit_status = WEXITSTATUS(main.status);
    	close (main.saved_stdout);
    	close (main.saved_stdin);
	}
 
  char *user_string;
  if (input)
    free(input);
  input = NULL;
	signal(SIGQUIT, SIG_DFL);
	signal(SIGINT, SIG_DFL);
  while (cat_counter)
  {
    user_string = get_next_line(0);
    free(user_string);
    cat_counter--;
  }
	return ;
}