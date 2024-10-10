
NAME = webserv_exe


SRC = srcs/main.cpp srcs/errors.cpp srcs/parsing.cpp \
	srcs/utils.cpp srcs/servers.cpp srcs/tester.cpp \
	srcs/Request.cpp srcs/Response.cpp srcs/locations.cpp \
	srcs/Client.cpp \
	srcs/Tcp_server.cpp srcs/RequestHandler.cpp srcs/WebServer.cpp


OBJ_DIR = object


OBJ = $(SRC:srcs/%.cpp=$(OBJ_DIR)/%.o)


CC = g++
FLAGS := -Wall -Wextra -Werror -std=c++98 -g


all: $(NAME)


$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)


$(NAME): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(NAME)

# Rule to compile .cpp files to .o files in the object folder
$(OBJ_DIR)/%.o: srcs/%.cpp | $(OBJ_DIR)
	$(CC) $(FLAGS) -c $< -o $@


clean:
	rm -rf $(OBJ_DIR)


fclean: clean
	rm -rf $(NAME)


re: fclean all

