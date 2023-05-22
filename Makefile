# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/11 21:40:31 by ykhadiri          #+#    #+#              #
#    Updated: 2023/05/22 16:35:11 by ykhadiri         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98
RM = rm -f
INCLUDES = tcp/includes/Tcp.hpp \
		irc/includes/Ircserv.hpp \
		irc/includes/Client.hpp \
		irc/includes/Channel.hpp \
		irc/includes/Command.hpp 

SRC = main.cpp \
	tcp/classes/Tcp.cpp \
	irc/classes/Ircserv.cpp\
	irc/classes/Client.cpp\
	irc/classes/Channel.cpp \
	irc/classes/Command.cpp

OBJ = $(SRC:.cpp=.o)

%.o: %.cpp $(INCLUDES)
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	$(CPP) $(OBJ) -lcurl -o $(NAME)

all : $(NAME)

clean :
	$(RM) $(OBJ)

fclean : clean
	$(RM) $(NAME)

re: fclean all

.PHONY: re all clean fclean