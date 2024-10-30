NAME			=	ircserv

CXX				=	c++

SRC_DIR			=	$(shell find srcs -type d)

OBJ_DIR			=	.build

vpath %.cpp $(foreach dir, $(SRC_DIR), $(dir):)

# library -----------------------------------------------------------

SRC			=	main.cpp server.cpp serverInit.cpp serverParse.cpp user.cpp

INC			=	./incl

OBJ			=	$(addprefix $(OBJ_DIR)/, $(SRC:%.cpp=%.o))

DEP			=	$(OBJ:%.o=%.d)

# Compilation flags -------------------------------------------------

CPPFLAGS			=	-Wall -Wextra -Werror -std=c++98
IFLAGS			=	-I incl/ -I /usr/includes
DFLAGS			=	-g -D_GLIBCXX_DEBUG

# Debug conditional -------------------------------------------------
DEBUG ?= 0
ifeq ($(DEBUG), 1)
	CPPFLAGS += $(DFLAGS)
endif

# Colors ------------------------------------------------------------

_GREY	=	$'\e[30m
_RED	=	$'\e[31m
_GREEN	=	$'\e[32m
_YELLOW	=	$'\e[33m
_BLUE	=	$'\e[34m
_PURPLE	=	$'\e[35m
_CYAN	=	$'\e[36m
_WHITE	=	$'\e[37m

# main part ---------------------------------------------------------

$(OBJ_DIR)/%.o : %.cpp
		@echo "Compiling $(_YELLOW)$@$(_WHITE) ...\t\t\t\c"
		@mkdir -p $(OBJ_DIR)
		@$(CXX) $(CPPFLAGS) $(IFLAGS) -MMD -c $< -o $@
		@echo "$(_GREEN)DONE$(_WHITE)"

all: $(NAME)

$(NAME): $(OBJ)
		@echo "-----\nCreating Binary File $(_YELLOW)$@$(_WHITE) ... \c"
		$(CXX) $(CPPFLAGS) $(OBJ) $(IFLAGS) -o $(NAME)
		@echo "$(_GREEN)DONE$(_WHITE)\n-----"

-include $(DEP)

clean:
		@echo "$(_WHITE)Deleting Objects Directory $(_YELLOW)$(OBJ_DIR)$(_WHITE) ... \c"
		@rm -rf $(OBJ_DIR)
		@echo "$(_GREEN)DONE$(_WHITE)\n-----"

fclean: clean
		@echo "$(_WHITE)Deleting Binary File $(_YELLOW)$(NAME)$(_WHITE) ... \c"
		@rm -f $(NAME)
		@echo "$(_GREEN)DONE$(_WHITE)\n-----"

re:		fclean all

.PHONY:		all bonus clean fclean re