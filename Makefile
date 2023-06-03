# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: qthierry <qthierry@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/31 18:39:31 by jvigny            #+#    #+#              #
#    Updated: 2023/06/03 15:17:44 by qthierry         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc
CFLAGS = -g -Wall -Wextra -Werror#-fsanitize=address
LIBS = -lreadline -lncurses
INCLUDES = -I$(HEADERS_DIR)

HEADERS_LIST = minishell.h builtins.h utils.h exec.h parsing.h structs.h \
				signals.h wildcard.h syntax_errors.h get_next_line.h heredocs.h
HEADERS_DIR = ./includes/
HEADERS = $(addprefix $(HEADERS_DIR), $(HEADERS_LIST))

BUILTINS = builtins/
SRC_BUILTINS = \
				cd.c \
				echo.c \
				env.c \
				exit.c \
				export.c \
				export2.c \
				pwd.c \
				unset.c

EXECUTION = execution/
SRC_EXECUTION = \
				binary_tree.c \
				exec_command.c \
				path.c \
				pipe.c \
				redirection.c


HEREDOCS = heredocs/
SRC_HEREDOCS = \
				expand_heredocs.c \
				get_next_line_utils.c \
				get_next_line.c \
				heredocs.c \
				prompt_here.c \
				random_name.c \

PARSING = parsing/
SRC_PARSING = \
				ast_rec.c \
				ast_utils.c \
				ast.c \
				expand_dollars.c \
				expand_dollars2.c \
				expand_utils.c \
				get_file_name.c \
				open_files.c \
				redirections.c \
				remove_quotes.c \
				second_parsing.c

SIGNALS = signals/
SRC_SIGNALS = \
				sig_handler.c \
				signals.c

SYNTAX_ERRORS = syntax_errors/
SRC_SYNTAX_ERRORS = \
				check_arguments_l_r.c \
				check_redirections.c \
				parsing_parenthesis.c \
				parsing_utils.c \
				syntax_errors.c \
				syntax_utils.c

UTILS = utils/
SRC_UTILS = \
				char_utils.c \
				error.c \
				free_tree.c \
				free_utils.c \
				ft_atouc.c \
				ft_calloc.c \
				ft_itoa.c \
				ft_split_quote.c \
				ft_split.c \
				ft_strcmp.c \
				ft_strdup.c \
				ft_strjoin.c \
				ft_strnstr.c \
				init.c \
				others_utils.c \
				skip_quote_par.c \
				tchar_join.c \
				tchar_ope.c \
				tchar_utils.c \
				utils.c

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
			$(addprefix $(SYNTAX_ERRORS), $(SRC_SYNTAX_ERRORS)) \
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
	mkdir -p $(OBJ_DIR)$(SYNTAX_ERRORS)

clean:
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(NAME)

re: fclean all

.PHONY : all clean fclean re
