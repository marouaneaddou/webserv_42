#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>

class Client
{
    public:
        Client(int new_socket, struct sockaddr_in new_addr);
        ~Client();
    private:
        int socket;
        struct sockaddr_in _sockaddr;

};