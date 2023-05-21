# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/31 18:39:31 by jvigny            #+#    #+#              #
#    Updated: 2023/05/21 03:40:36 by qthierry         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc
CFLAGS = -g -Wall -Wextra #-fsanitize=address# -Werror
LIBS = -lreadline -lncurses
INCLUDES = -I$(HEADERS_DIR)

HEADERS_LIST = minishell.h builtins.h utils.h exec.h parsing.h structs.h \
				signals.h wildcard.h
HEADERS_DIR = ./includes/
HEADERS = $(addprefix $(HEADERS_DIR), $(HEADERS_LIST))

EXECUTION = execution/
SRC_EXECUTION = \
				exec_command.c \
				binary_tree.c \
				redirection.c \
				pipe.c \
				path.c

BUILTINS = builtins/
SRC_BUILTINS = \
				echo.c \
				env.c \
				export.c \
				export2.c \
				exit.c \
				pwd.c \
				unset.c \
				canonic_path.c \
				cd.c

UTILS = utils/
SRC_UTILS = \
			ft_calloc.c \
			ft_strdup.c \
			ft_atouc.c \
			ft_split_quote.c \
			ft_split.c \
			ft_strjoin.c \
			ft_strcmp.c \
			utils.c \
			init.c \
			free_tree.c \
			ft_strnstr.c \
			tchar_join.c \
			tchar_ope.c \
			tchar_utils.c \
			free_utils.c \
			error.c

PARSING = parsing/
SRC_PARSING = \
			ast.c \
			ast_rec.c \
			ast_utils.c \
			expand_dollars.c \
			parsing_parenthesis.c \
			parsing_utils.c \
			remove_quotes.c \
			second_parsing.c \
			syntax_errors.c \
			redirections.c \
			get_file_name.c \
			open_files.c \
			utils2.c

HEREDOCS = heredocs/
SRC_HEREDOCS = \
			expand_heredocs.c \
			get_next_line_utils.c \
			get_next_line.c \
			heredocs.c \
			prompt_here.c \
			random_name.c \

SIGNALS = signals/
SRC_SIGNALS = \
			signals.c \
			sig_handler.c

WILDCARD = wildcard/
SRC_WILDCARD = \
			file_list.c \
			pattern_finder.c \
			prefix.c \
			replace_input.c \
			suffix.c \
			wild_utils.c \
			wildcard.c
			
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

bonus:	all

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
