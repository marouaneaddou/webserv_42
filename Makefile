NAME = webserv_exe
SRC = srcs/main.cpp srcs/errors.cpp srcs/parsing.cpp \
	srcs/utils.cpp srcs/servers.cpp srcs/tester.cpp \
	srcs/Request.cpp srcs/Response.cpp srcs/locations.cpp  \
	srcs/Client.cpp srcs/PhpCgiHandler.cpp srcs/StaticFileHandler.cpp srcs/Tcp_server.cpp srcs/RequestHandler.cpp \
	srcs/WebServer.cpp \

OBJ = $(SRC:.cpp=.o)
CC = g++
FLAGS := -Wall -Wextra -Werror -std=c++98 -g -fsanitize=address

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
