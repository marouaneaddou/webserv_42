#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include "HTTPRequest.hpp"

class HTTPRequest;
class Client
{
    public:
        Client(int new_socket, struct sockaddr_in new_addr);
        ~Client();
        bool read_request(int socket);
        HTTPRequest *request;
      
    private:
        int socket;
        struct sockaddr_in _sockaddr;
        std::string raw_request;


};