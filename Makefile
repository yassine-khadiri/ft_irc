# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ykhadiri <ykhadiri@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/11 21:40:31 by ykhadiri          #+#    #+#              #
#    Updated: 2023/05/24 17:20:25 by ykhadiri         ###   ########.fr        #
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
	src/cmds/inviteCommand.cpp \
	src/cmds/botCommand.cpp \
	src/cmds/joinCommand.cpp \
	src/cmds/kickCommand.cpp \
	src/cmds/modeCommand.cpp \
	src/cmds/msgCommand.cpp \
	src/cmds/nickCommand.cpp \
	src/cmds/partCommand.cpp \
	src/cmds/topicCommand.cpp \
	src/cmds/userCommand.cpp \
	src/cmds/quitCommand.cpp \
	src/cmds/passCommand.cpp


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