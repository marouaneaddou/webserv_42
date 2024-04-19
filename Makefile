NAME = webserv
SRC = srcs/main.cpp
OBJ = $(SRC:.cpp=.o)
CC = c++
FLAGS := -Wall -Wextra -Werror -std=c++98

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME) && mkdir objs && mv $(OBJ) objs/

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@



mvobj:


clean:
	rm -rf objs

fclean: clean
	rm -f $(NAME)

re: fclean all
