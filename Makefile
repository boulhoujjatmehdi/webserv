# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aachfenn <aachfenn@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/16 11:15:12 by aachfenn          #+#    #+#              #
#    Updated: 2023/12/16 11:15:56 by aachfenn         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC     = $(wildcard parcing/*.cpp)

CXXFLAGS   = -Wall -Wextra -Werror -std=c++98

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
