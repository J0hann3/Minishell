# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/31 18:39:31 by jvigny            #+#    #+#              #
#    Updated: 2023/03/30 03:18:07 by jvigny           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -g -Werror
LIBS = -lreadline -lncurses
INCLUDES = -I$(HEADERS_DIR)

HEADERS_LIST = minishell.h builtins.h utils.h exec.h parsing.h
HEADERS_DIR = ./includes/
HEADERS = $(addprefix $(HEADERS_DIR), $(HEADERS_LIST))

EXECUTION = execution/
SRC_EXECUTION =	exec_command.c \
				binary_tree.c

BUILTINS = builtins/
SRC_BUILTINS =	echo.c \
				env.c \
				export.c \
				exit.c \
				pwd.c \
				unset.c \
				cd.c

UTILS = utils/
SRC_UTILS = ft_calloc.c \
			ft_strdup.c \
			ft_atouc.c \
			ft_split.c \
			ft_strjoin.c \
			ft_strcmp.c \
			utils.c \
			init.c \
			free_tree.c

PARSING = parsing/
SRC_PARSING = syntax_errors.c \
			parsing_utils.c \
			ast.c \
			ast_utils.c \
			parsing_parenthesis.c \
			utils2.c
			
SRC_LIST =	$(addprefix $(BUILTINS), $(SRC_BUILTINS)) \
			$(addprefix $(EXECUTION), $(SRC_EXECUTION)) \
			$(addprefix $(PARSING), $(SRC_PARSING)) \
			$(addprefix $(UTILS), $(SRC_UTILS)) \
			main.c \
			error.c

SRC_DIR = ./src/
SRC = $(addprefix $(SRC_DIR), $(SRC_LIST))


OBJ_DIR = ./obj/
OBJ_LIST = $(patsubst %.c, %.o, $(SRC_LIST))
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_LIST))

all:	$(NAME)

tester: $(NAME)
	./tester.sh

testerp: $(NAME)
	./tester.sh -p

run: $(NAME)
	./$(NAME)

vrun: $(NAME)
	valgrind --track-fds=all --trace-children=yes ./$(NAME)

$(NAME):	$(OBJ_DIR) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) $(INCLUDES) -o $(NAME)

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c $(HEADERS) Makefile
	$(CC) $(CFLAGS) -c $(INCLUDES) $(LIBS) $< -o $@
	
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)$(BUILTINS)
	mkdir -p $(OBJ_DIR)$(EXECUTION)
	mkdir -p $(OBJ_DIR)$(PARSING)
	mkdir -p $(OBJ_DIR)$(UTILS)

clean:
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(NAME)

re: fclean all

.PHONY : all clean fclean re
