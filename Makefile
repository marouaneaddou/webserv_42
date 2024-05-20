NAME = webserv
SRC = srcs/main.cpp srcs/errors.cpp srcs/parsing.cpp \
		srcs/reading.cpp srcs/utils.cpp srcs/servers.cpp
OBJ = $(SRC:.cpp=.o)
CC = c++
FLAGS := -Wall -Wextra -Werror -std=c++98

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@




clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
