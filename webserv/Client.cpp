#include "Client.hpp"

// Client::Client(int new_socket, struct sockaddr_in new_addr)
// {
//     socket = new_socket;
//     _sockaddr = new_addr;
// }

Client::Client(int &new_socket)
{
    socket = new_socket;
    _request = new Request();
    _response = new Response();
};

Client::~Client(){
    delete _request;
    delete _response;
}

