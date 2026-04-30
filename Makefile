# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gaducurt <gaducurt@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/15 14:12:34 by amerzone          #+#    #+#              #
#    Updated: 2026/04/30 10:55:58 by gaducurt         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_irc

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
		sources/command/NICK.cpp \
		sources/command/PASS.cpp \
		sources/command/USER.cpp \
		sources/command/JOIN.cpp \
		sources/command/NOTICE.cpp \
		sources/command/PRIVMSG.cpp
		

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
