# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: hbouqssi <hbouqssi@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/11 21:40:31 by ykhadiri          #+#    #+#              #
#    Updated: 2023/04/20 12:28:25 by hbouqssi         ###   ########.fr        #
#    Updated: 2023/04/23 19:08:19 by ykhadiri         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CC = c++
CFLAGS = -Wall -Wextra -Werror -std=c++98
RM = rm -f
INCLUDES = tcp/includes/Ircserv.hpp \
		irc/includes/Tcp.hpp\
		irc/includes/IrcClient.hpp
INCLUDES = tcp/includes/Tcp.hpp \
		irc/includes/Ircserv.hpp \
		irc/includes/Client.hpp \
		irc/includes/Command.hpp \
		irc/includes/Channel.hpp

SRC = main.cpp \
	tcp/classes/Tcp.cpp \
	irc/classes/Ircserv.cpp\
	irc/classes/IrcClient.cpp
	irc/classes/Ircserv.cpp \
	irc/classes/Client.cpp \
	irc/classes/Command.cpp \
	irc/classes/Channel.cpp

OBJ = $(SRC:.cpp=.o)

%.o: %.cpp $(INCLUDES)
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

all : $(NAME)

clean :
	$(RM) $(OBJ)

fclean : clean
	$(RM) $(NAME)

re: fclean all

.PHONY: re all clean fclean