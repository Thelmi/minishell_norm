/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/27 11:27:51 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/27 17:28:17 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static char	*heredoc_quotes(t_main main)
{
	int		i;
	char	*tmp2;
	char	*tmp;

	i = 0;
	while (main.heredoc->argv[i] && main.heredoc->argv[i] != ' ')
	{
		i++;
	}
	tmp = ft_substr(main.heredoc->argv, 0, i);
	if (tmp && tmp[0] == '\"' && tmp[ft_strlen(tmp) - 1] == '\"')
	{
		tmp2 = ft_substr(main.heredoc->argv, 1, ft_strlen(tmp) - 2);
		free(tmp);
		tmp = tmp2;
	}
	if (tmp && tmp[0] == '\'' && tmp[ft_strlen(tmp) - 1] == '\'')
	{
		tmp2 = ft_substr(main.heredoc->argv, 1, ft_strlen(tmp) - 2);
		free(tmp);
		tmp = tmp2;
	}
	return (tmp);
}

static char	*join_input(char *input, char *read)
{
	char	*tmp_input;
	char	*tmp_input2;

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
	return (input);
}

static char	*user_input_heredoc(t_main main, char *input, char *tmp2)
{
	char	*read;
	char	*tmp_input;

	read = NULL;
	write(1, "> ", 2);
	read = get_next_line(0);
	tmp_input = ft_substr(read, 0, ft_strlen(read) - 1);
	free(read);
	read = tmp_input;
	if (read == NULL)
		return (NULL);
	if (num_strncmp(read, tmp2) == 0)
	{
		free(read);
		read = NULL;
		return (NULL);
	}
	if (main.heredoc->next == NULL)
		input = join_input(input, read);
	else
		return (free(read), tmp2);
	free(read);
	return (input);
}

static char	*verify_heredoc(t_main main, struct heredoc **heredoc, char *input,
		char *tmp2)
{
	char		*tmp;
	t_heredoc	*tmp_h;

	tmp = NULL;
	while (1)
	{
		tmp = user_input_heredoc(main, input, tmp2);
		if (!tmp)
			break ;
		if (tmp == tmp2)
		{
			if (input)
				free(input);
			input = NULL;
			continue ;
		}
		input = tmp;
	}
	free(tmp2);
	tmp_h = main.heredoc;
	*heredoc = (*heredoc)->next;
	free(tmp_h);
	tmp_h = NULL;
	return (input);
}

char	*heredoc_exec(t_main main, int *last_exit_status, int *has_heredoc)
{
	char	*tmp2;
	char	*input;

	input = 0;
	if (*(main.has_heredoc) || !main.heredoc || !(main.heredoc->argv[0]))
		return (NULL);
	while (main.heredoc)
	{
		tmp2 = heredoc_quotes(main);
		if (tmp2 == NULL)
		{
			perror("Error allocating memory for heredoc delimiter");
			*last_exit_status = 1;
			return (0);
		}
		*has_heredoc = 1;
		if (input)
			free(input);
		input = NULL;
		input = verify_heredoc(main, &(main.heredoc), input, tmp2);
	}
	freeheredoc(main.heredoc);
	*(main.has_heredoc) = 1;
	return (input);
}
