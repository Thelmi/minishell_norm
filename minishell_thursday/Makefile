# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thelmy <thelmy@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/22 14:53:08 by krazikho          #+#    #+#              #
#    Updated: 2024/09/25 23:00:47 by thelmy           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3
LDFLAGS = -L/opt/vagrant/embedded/lib/ -lreadline -I/opt/vagrant/embedded/include/readline

NAME_SRCS = main.c signals.c execute/execute_command.c execute/execute_builtin.c execute/modify_args.c execute/parse_execute.c\
			utils/string_functions.c utils/modify_utils.c utils/env_related_utils.c utils/export_related_utils.c utils/free.c utils/env_related_utils2.c\
			utils/string_functions2.c utils/other_utils.c utils/cd_utils.c utils/echo_utils.c utils/get_next_line.c utils/get_next_line_utils.c\
			./builtins/pwd.c ./builtins/exit_builtin.c ./builtins/env.c ./builtins/export_no_args.c ./builtins/echo.c\
			./builtins/export_with_args.c ./builtins/unset.c ./builtins/cd.c\
			./mini_libft/partone.c ./execution/execution.c ./execution/heredoc.c\

NAME_OBJS = $(NAME_SRCS:.c=.o)

all: $(NAME)

$(NAME): $(NAME_OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(NAME_OBJS) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(NAME_OBJS)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re