/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 14:58:22 by krazikho          #+#    #+#             */
/*   Updated: 2024/09/23 21:00:18 by thelmy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *concat_var_value(const char *variable, const char *value) {
    size_t var_len = ft_strlen(variable);
    size_t val_len = ft_strlen(value);
    char *result = malloc(var_len + val_len + 2);

    if (result == NULL) {
        perror("malloc");
        return NULL;
    }
    ft_strlcpy(result, variable, var_len + 1);
    result[var_len] = '=';
    ft_strlcpy(result + var_len + 1, value, val_len + 1);
    return result;
}

void fill_env(t_env **env, char **ev)
{
    t_env *tmp;

    tmp = *(env);
    while (tmp)
    {
        tmp->ev = ev;
        tmp = tmp->next;
    }
}

static char **convert_env(t_env **env) {
    int count = 0;
    t_env *temp = *env; 

    while (temp) {
        count++;
        temp = temp->next;
    }

    char **env_array = malloc((count + 1) * sizeof(char *));
    if (env_array == NULL) {
        perror("malloc");
        return NULL;
    }

    temp = *env;
    int i = 0;
    while (temp) {
        env_array[i] = concat_var_value(temp->variable, temp->value);
        if (env_array[i] == NULL) {
            int j = 0;
            while (j < i) {
                free(env_array[j]);
                j++;
            }
            free(env_array);
            return NULL;
        }
        i++;
        temp = temp->next;
    }

    env_array[count] = NULL;
    return env_array;
}


static void	initialize_shell(char **ev, t_env **envir, t_export **exp,
		t_context *context)
{
	context->last_exit_status = 0;
	configure_terminal_behavior();

	*envir = storing_env(ev);
	*exp = storing_export(ev);
}

static void	command_loop(t_env **envir, t_export **exp,
		t_context *context)
{
	char	*command;
	while (1)
	{
        setup_signals();
		command = readline("minishell$ ");
		//command = get_next_line(0);
		if (command == NULL)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (*command)
		{
			signal(SIGQUIT, SIG_IGN);
    			signal(SIGINT, SIG_IGN);
			(*envir)->ev = convert_env(envir);
            fill_env(envir, (*envir)->ev);
			add_history(command);
            t_main x = parsecmd(command, *envir, &context->last_exit_status);
            x.command = command;
            if (x.cmd)
                runcmd(x, envir, exp, &context->last_exit_status);
            if (x.cmd)
                freecmd(x.cmd, 0);
            if (envir && *envir && (*envir)->ev)
                free_double_pointer((*envir)->ev);
		}
		free(command);
	}
	free_env(*envir);
	free_export(*exp);
}

int	main(int ac, char **av, char **ev)
{
	t_env		*envir;
	t_export	*exp;
	t_context	context;

	(void)ac;
	(void)av;
	initialize_shell(ev, &envir, &exp, &context);
	command_loop(&envir, &exp, &context);
	return (context.last_exit_status);
}
