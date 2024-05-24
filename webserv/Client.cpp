#include "Client.hpp"

// Client::Client(int new_socket, struct sockaddr_in new_addr)
// {
//     socket = new_socket;
//     _sockaddr = new_addr;
// }

Client::Client(int &new_socket /*class config*/)
{
    socket = new_socket;
    _check = false;
    // _request = new Request(/*class config*/);
    // _response = new Response(/*class config*/);
};

Client::~Client(){
    // delete _request;
    // delete _response;
}

void Client::setCheck()
{
    _check = true;
}

bool Client::getCheck() const
{
    return _check;
}

Response &Client::getResponse()
{
    return _response;
}
