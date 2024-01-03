# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aachfenn <aachfenn@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/16 11:15:12 by aachfenn          #+#    #+#              #
#    Updated: 2024/01/03 15:24:07 by aachfenn         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# SRC     = $(wildcard parcing/*.cpp)
SRC     = $(wildcard **/*.cpp)

CXXFLAGS   = -g -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address

OBJS    = ${SRC:.cpp=.o}

RM      = rm -f

NAME    = exxx

${NAME} :	${OBJS} $(wildcard *.hpp)
		c++ ${CXXFLAGS} ${OBJS} -o ${NAME}

all     : ${NAME}

clean   :
		${RM} ${OBJS}

fclean  : clean
		${RM} ${NAME}

re      : fclean all
