# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/31 18:39:31 by jvigny            #+#    #+#              #
#    Updated: 2023/05/05 00:08:19 by qthierry         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc
CFLAGS = -g -Wall -Wextra #-fsanitize=address -Werror
LIBS = -lreadline -lncurses
INCLUDES = -I$(HEADERS_DIR)

HEADERS_LIST = minishell.h builtins.h utils.h exec.h parsing.h structs.h \
				signals.h wildcard.h
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
			ft_split_quote.c \
			ft_split.c \
			ft_strjoin.c \
			ft_strcmp.c \
			utils.c \
			init.c \
			free_tree.c \
			error.c

PARSING = parsing/
SRC_PARSING = syntax_errors.c \
			parsing_utils.c \
			ast.c \
			ast_utils.c \
			parsing_parenthesis.c \
			second_parsing.c \
			open_fd.c \
			expand.c \
			remove_quotes.c \
			utils2.c

HEREDOCS = heredocs/
SRC_HEREDOCS = heredocs.c \
			prompt_here.c \
			expand_heredocs.c \
			get_next_line.c \
			get_next_line_utils.c \

SIGNALS = signals/
SRC_SIGNALS = signals.c

WILDCARD = wildcard/
SRC_WILDCARD = wildcard.c
			
SRC_LIST =	$(addprefix $(BUILTINS), $(SRC_BUILTINS)) \
			$(addprefix $(EXECUTION), $(SRC_EXECUTION)) \
			$(addprefix $(PARSING), $(SRC_PARSING)) \
			$(addprefix $(UTILS), $(SRC_UTILS)) \
			$(addprefix $(SIGNALS), $(SRC_SIGNALS)) \
			$(addprefix $(HEREDOCS), $(SRC_HEREDOCS)) \
			$(addprefix $(WILDCARD), $(SRC_WILDCARD)) \
			main.c

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
	valgrind --leak-check=full --track-fds=yes --trace-children=yes --show-leak-kinds=all --track-origins=yes --suppressions=suppr.valgrind ./$(NAME)

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
	mkdir -p $(OBJ_DIR)$(SIGNALS)
	mkdir -p $(OBJ_DIR)$(HEREDOCS)
	mkdir -p $(OBJ_DIR)$(WILDCARD)

clean:
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(NAME)

re: fclean all

.PHONY : all clean fclean re
