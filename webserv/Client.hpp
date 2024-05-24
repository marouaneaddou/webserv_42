#pragma once

# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include "Request.hpp"
# include "Response.hpp"

class Request;
// class Response;
class Client
{
    public:
        Client(int &new_socket);
        ~Client();
        void setCheck();
        bool getCheck() const;
        Response& getResponse();
        // Client(int new_socket, struct sockaddr_in new_addr);
        // std::string		getResponse(void); 
        Request _request;
        Response _response;
    private:
        int socket;
        bool _check;
        struct sockaddr_in _sockaddr;
        std::string raw_request;
        std::string method;

};