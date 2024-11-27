NAME			=	ircserv

INC				=	includes/
BIN				=	srcs/bin

SRCS			=	$(shell find srcs -type f -name "*.cpp")
INCS			=	$(shell find inc -type f -name "*.hpp")
OBJS			=	$(SRCS:srcs/%.cpp=srcs/bin/%.o)
HOBJS			=	$(INCS:includes/%.hpp)

CC				=	c++
RM				=	rm -rf
CPPFLAGS		=	-std=c++98 -Wall -Wextra -Werror

all:			$(NAME)

$(BIN):
	@mkdir $(BIN)

$(NAME):	$(BIN) $(OBJS) $(HOBJS)
			$(CC) $(CPPFLAGS) $(OBJS) -o $(NAME)

clean:
			$(RM) $(OBJS) $(HOBJS)
			$(RM) $(BIN)

fclean:			clean
				@$(RM) $(NAME)

re:				fclean $(NAME)

$(BIN)%.o: srcs/%.cpp
	@mkdir -p $(shell dirname $@)
	@$(CC) $(CPPFLAGS) -c $< -o $@ -I$(INC)


.PHONY:			all clean fclean re bonus