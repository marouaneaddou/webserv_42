#pragma once

# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include "Request.hpp"
# include "Response.hpp"
#include "../includes/servers.hpp"

class Request;
// class Response;
class Client
{
    public:
        Client(int &new_socket);
        ~Client();
        void setCheck();
        void setConf(Servers &server);
        bool getCheck() const;
        Response& getResponse();
        Servers getServer();
        // Client(int new_socket, struct sockaddr_in new_addr);
        // std::string		getResponse(void); 
        Request _request;
        Response _response;
        int socket;
    private:
        bool _check;
        Servers _server;
        struct sockaddr_in _sockaddr;
        std::string raw_request;
        std::string method;

};