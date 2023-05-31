# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/11 21:40:31 by ykhadiri          #+#    #+#              #
#    Updated: 2023/05/31 19:14:52 by ykhadiri         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CPP = c++
CPPFLAGS = -Wall -Wextra -Werror -std=c++98
RM = rm -f
INC = inc/Tcp.hpp \
		inc/Ircserv.hpp \
		inc/Client.hpp \
		inc/Channel.hpp \
		inc/Command.hpp

SRC = main.cpp \
	src/Tcp.cpp \
	src/Ircserv.cpp \
	src/Client.cpp\
	src/Channel.cpp \
	src/Command.cpp \
	src/commands/invite.cpp \
	src/commands/bot.cpp \
	src/commands/join.cpp \
	src/commands/kick.cpp \
	src/commands/mode.cpp \
	src/commands/msg.cpp \
	src/commands/nick.cpp \
	src/commands/part.cpp \
	src/commands/topic.cpp \
	src/commands/user.cpp \
	src/commands/quit.cpp \
	src/commands/pass.cpp

OBJ = $(SRC:.cpp=.o)

%.o: %.cpp $(INC)
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