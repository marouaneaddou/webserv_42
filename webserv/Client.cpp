#include "Client.hpp"


Client::Client(int new_socket, struct sockaddr_in new_addr)
{
    socket = new_socket;
    _sockaddr = new_addr;
}

Client::~Client(){}