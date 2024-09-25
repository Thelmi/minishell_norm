/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mrhelmy <mrhelmy@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/25 20:32:22 by thelmy            #+#    #+#             */
/*   Updated: 2024/09/15 14:55:27 by mrhelmy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	get_and_update(char *buffer, char **str)
{
	int	i;

	*str = gnl_strjoin(*str, buffer);
	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (buffer[i] == '\n')
		gnl_memmove(buffer, buffer + i + 1, enhanced_strlen(buffer + i + 1, 3)
			+ 1);
	else
		buffer[0] = '\0';
}

char	*get_next_line(int fd)
{
	static char	buffer[BUFFER_SIZE + 1u];
	char		*str;
	int			bytes;

	bytes = 1;
	str = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0 || BUFFER_SIZE > INT_MAX)
		return (NULL);
	while (!gnl_strchr(str, '\n') && bytes > 0)
	{
		if (buffer[0] == '\0')
		{
			bytes = read(fd, buffer, BUFFER_SIZE);
			if (bytes == -1 && gnl_free_str(&str))
				return (NULL);
			if (bytes == 0)
				return (str);
			buffer[bytes] = '\0';
		}
		get_and_update(buffer, &str);
	}
	return (str);
}
