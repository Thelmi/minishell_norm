/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhelmy <mrhelmy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 03:10:43 by thelmy            #+#    #+#             */
/*   Updated: 2024/09/26 19:54:08 by mrhelmy          ###   ########.fr       */
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


int check_quotes(char *str)
{
    int i;
    int double_counter;
    int single_counter;

    i = 0;
    double_counter = 0;
    single_counter = 0;
    while (str[i])
    {
        if (str[i] == '\"')
            double_counter++;
        if (str[i] == '\'')
            single_counter++;
        i++;
    }
    if (double_counter%2 != 0)
        return (1);
    if (single_counter%2 != 0)
        return (1);
    return (0);
}

void    free_wrong_parsing(char *s, struct cmd *cmd,
    t_main main, int *last_exit_status)
{
	write(2, "minishell: syntax error: unexpected token ", 42);
	write(2, s, ft_strlen(s));
	write (1, "\n", 1);
	*last_exit_status = 2;
	freecmd(cmd, 1);
	freeheredoc(main.heredoc);
}
