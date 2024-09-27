/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_no_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 18:10:11 by mrhelmy           #+#    #+#             */
/*   Updated: 2024/09/25 22:49:36 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	listsize(t_export *export)
{
	int			count;
	t_export	*tmp;

	count = 0;
	tmp = export;
	while (tmp != NULL)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

void	sort_array(t_export **export_arr, int count)
{
	int			i;
	int			j;
	t_export	*tmp;

	i = 0;
	j = 0;
	while (i < count - 1)
	{
		j = i + 1;
		while (j < count)
		{
			if (num_strncmp(export_arr[i]->variable,
					export_arr[j]->variable) > 0)
			{
				tmp = export_arr[i];
				export_arr[i] = export_arr[j];
				export_arr[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	printing_export(t_export **export_arr, int export_count)
{
	int	i;

	i = 0;
	while (i < export_count)
	{
		printf("declare -x %s", export_arr[i]->variable);
		if (export_arr[i]->value)
			printf("=\"%s\"", export_arr[i]->value);
		else
			printf("=\"\"");
		printf("\n");
		i++;
	}
}

static t_export	**create_export_array(t_export *export, int export_count,
						int *last_exit_status)
{
	t_export	**export_arr;
	t_export	*tmp;
	int			i;

	export_arr = malloc(sizeof(t_export *) * export_count);
	if (!export_arr)
	{
		perror("Error allocating memory for export array");
		*last_exit_status = 1;
		return (NULL);
	}
	tmp = export;
	i = 0;
	while (i < export_count)
	{
		export_arr[i] = tmp;
		tmp = tmp->next;
		i++;
	}
	sort_array(export_arr, export_count);
	return (export_arr);
}

void	export_no_arg(t_export *export, int *last_exit_status)
{
	t_export	**export_arr;
	int			export_count;

	export_count = listsize(export);
	if (export_count == 0)
		return ;
	export_arr = create_export_array(export, export_count, last_exit_status);
	if (!export_arr)
		return ;
	printing_export(export_arr, export_count);
	free(export_arr);
}

//void	export_no_arg(t_export *export, int *last_exit_status)
//{
//	t_export	*tmp;
//	t_export	**export_arr;
//	int			export_count;
//	int			i;

//	export_count = listsize(export);
//	if (export_count == 0)
//		return ;
//	export_arr = malloc(sizeof(t_export *) * export_count);
//	if (!export_arr)
//	{
//		perror("Error allocating memory for export array");
//		*last_exit_status = 1;
//		return ;
//	}
//	i = 0;
//	tmp = export;
//	while (i < export_count)
//	{
//		export_arr[i] = tmp;
//		tmp = tmp->next;
//		i++;
//	}
//	sort_array(export_arr, export_count);
//	i = 0;
//	printing_export(export_arr, export_count);
//	free(export_arr);
//}
