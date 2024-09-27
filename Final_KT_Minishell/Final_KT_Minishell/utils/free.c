/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/19 16:17:37 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/27 17:50:14 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	free_exp_node(t_export *node)
{
	if (node)
	{
		free(node->variable);
		free(node->value);
		free(node);
	}
}

void	free_env_node(t_env *node)
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

void	freeheredoc(struct heredoc *heredoc)
{
	struct heredoc	*tmp;

	while (heredoc)
	{
		tmp = heredoc;
		heredoc = heredoc->next;
		free(tmp);
		tmp = NULL;
	}
}

void	cleanup(t_main *main)
{
	if (main->cmd)
		freecmd(main->cmd, 0);
	if (main->heredoc)
		freeheredoc(main->heredoc);
}
