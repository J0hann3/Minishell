# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: jvigny <jvigny@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/31 18:39:31 by jvigny            #+#    #+#              #
#    Updated: 2023/03/21 18:21:02 by jvigny           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

CC = gcc
CFLAGS = -Wall -Wextra -g #-Werror
INCLUDES = -I$(HEADERS_DIR) -lreadline

HEADERS_LIST = minishell.h builtins.h utils.h exec.h
HEADERS_DIR = ./includes/
HEADERS = $(addprefix $(HEADERS_DIR), $(HEADERS_LIST))

EXECUTION = execution/
SRC_EXECUTION =	exec_command.c

BUILTINS = builtins/
SRC_BUILTINS =	echo.c \
				env.c \
				export.c \
				exit.c \
				pwd.c \
				unset.c \
				cd.c \

SRC_UTILS = ft_calloc.c \
			ft_strdup.c \
			ft_strlen.c \
			ft_atouc.c \
			ft_split.c \
			ft_strjoin.c \
			ft_strcmp.c \
			utils.c

SRC_LIST =	$(addprefix $(BUILTINS), $(SRC_BUILTINS)) \
			$(addprefix $(EXECUTION), $(SRC_EXECUTION)) \
			$(SRC_UTILS) \
			main.c \
			error.c \
			init.c \

SRC_DIR = ./src/
SRC = $(addprefix $(SRC_DIR), $(SRC_LIST))

	
OBJ_DIR = ./obj/
OBJ_LIST = $(patsubst %.c, %.o, $(SRC_LIST))
OBJ = $(addprefix $(OBJ_DIR), $(OBJ_LIST))

all:	$(NAME)

$(NAME):	$(OBJ_DIR) $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(INCLUDES) -o $(NAME)

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c $(HEADERS) Makefile
	$(CC) $(CFLAGS) -c $(INCLUDES) $< -o $@
	
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)
	mkdir -p $(OBJ_DIR)$(BUILTINS)
	mkdir -p $(OBJ_DIR)$(EXECUTION)

clean:
	rm -rf $(OBJ_DIR)

fclean:	clean
	rm -f $(NAME)

re: fclean all

.PHONY : all clean fclean re
