# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/15 14:12:34 by amerzone          #+#    #+#              #
#    Updated: 2026/05/11 10:30:28 by gaducurt         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = 1

CXX = c++

#FLAGS = -Wall -Wextra -Werror -MMD -std=c++98 -g3
FLAGS = -MMD -std=c++98 -g3

INC = includes

OBJDIR = obj/

SRC =	main.cpp \
		sources/Client.cpp \
		sources/Server.cpp \
		sources/Channel.cpp \
		sources/error.cpp \
		sources/FunctionError.cpp \
		sources/parsingArguments.cpp \
		sources/tools.cpp \
		sources/command/JOIN.cpp \
		sources/command/KICK.cpp \
		sources/command/MODE.cpp \
		sources/command/NICK.cpp \
		sources/command/NOTICE.cpp \
		sources/command/PASS.cpp \
		sources/command/PING_PONG.cpp \
		sources/command/PRIVMSG.cpp \
		sources/command/TOPIC.cpp \
		sources/command/USER.cpp \
		sources/command/WHO.cpp
		

OBJ := $(addprefix $(OBJDIR), $(SRC:.cpp=.o))
DEPS = $(OBJ:.o=.d)

all : $(NAME)

$(NAME) : $(OBJ)
	$(CXX) $(FLAGS) -I $(INC) -o $(NAME) $(OBJ)

$(OBJDIR)%.o : %.cpp | $(OBJDIR)
	@mkdir -p $(dir $@)
	$(CXX) $(FLAGS) -I $(INC) -o $@ -c $<

$(OBJDIR):
	@mkdir -p $(OBJDIR)

clean :
	rm -rf $(OBJDIR)

fclean : clean
	rm -f $(NAME)

re : fclean all

.PHONY: all clean fclean re

-include $(DEPS)
