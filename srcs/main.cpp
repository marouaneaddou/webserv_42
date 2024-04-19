/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aech-che <aech-che@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:35:07 by aech-che          #+#    #+#             */
/*   Updated: 2024/04/19 11:37:39 by aech-che         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"



#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int server_socket;
    int client_socket;

    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Creating socket file descriptor
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Socket creation failed" << std::endl;
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        return -1;
    }

    // Listening
    if (listen(server_socket, 5) < 0) {
        std::cerr << "Listen failed" << std::endl;
        return -1;
    }

    std::cout << "visit -> http://c1r5p3.1337.ma:8080/" << std::endl;

    while (true) {
        if ((client_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            std::cerr << "Accept failed" << std::endl;
            return -1;
        }
        char buffer[1024] = {0};
        read(client_socket, buffer, 1024);

        std::ifstream file("html/index.html");
        std::stringstream buffer2;
        buffer2 << file.rdbuf();
        std::string html_content = buffer2.str();



        std::string response = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n" + html_content;
        send(client_socket, response.c_str(), response.length(), 0);
        close(client_socket);
    }

    close(server_socket);

    return 0;
}

// char arr[200]="HTTP/1.1 200 OK\nContent-Type:text/html\nContent-Length: 16\n\n<h1>testing</h1>";
// int send_res=send(sock,arr,sizeof(arr),0);
