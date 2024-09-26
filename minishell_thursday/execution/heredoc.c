#include "../minishell.h"

char *heredoc_exec(t_main main, int *last_exit_status, int *has_heredoc)
{
    int i;
    char *tmp3;
    char *tmp2;
    char *read = NULL;
    char *tmp_input;
    char *tmp_input2;
    char *input = 0;
    t_heredoc *tmp_h;

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
            return 0;
        }
        *has_heredoc = 1;
        while (1)
        {
            write (1, "> ", 2);
            read = get_next_line(0);
            tmp_input = ft_substr(read, 0, ft_strlen(read) - 1);
            free (read);
            read = tmp_input;
            if (read == NULL)
                break ;
            if (num_strncmp(read, tmp2) == 0)
            {
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
    return (input);
}