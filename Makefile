# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: amerzone <amerzone@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2026/04/15 14:12:34 by amerzone          #+#    #+#              #
#    Updated: 2026/04/15 15:00:29 by amerzone         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ft_irc

CXX = c++

FLAGS = -Wall -Wextra -Werror -MMD -std=c++98 -g3

INC = includes

OBJDIR = obj/

SRC =	main.cpp \
		sources/parsingArguments.cpp \
		sources/Server.cpp

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