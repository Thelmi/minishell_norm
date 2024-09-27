/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gettoken.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhelmy <mrhelmy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 03:10:43 by thelmy            #+#    #+#             */
/*   Updated: 2024/09/26 20:39:00 by mrhelmy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	get_token_extend(char **s, char *es, char *whitespace,
		char *symbols)
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

static int	get_token_extend_red(char **s, char *es, char *whitespace,
		char *symbols)
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
