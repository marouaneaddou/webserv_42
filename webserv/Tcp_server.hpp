#pragma once

#include <unistd.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/_select.h>
#include <vector>
#include <algorithm>
#include <iostream>

// #include "Client.hpp"


#define MAX_CONNECTIONS 2048
#define BACKLOGS 512
#define FDS 20
class TCPserver
{
    public:
        TCPserver(std::vector<int> port);
        ~TCPserver();
        fd_set current_Rsockets, ready_Rsockets;
        fd_set current_Wsockets, ready_Wsockets;
        void exit_error(const std::string err_msg);
        int acceptConnection(int socket);
        // std::vector<class Client> getClients() ;
        std::vector<int> getSocket() ;
        std::vector<int> getCliSocket() ;
    private:
        int _backlogs;
        std::vector<int> _Port;
        std::vector<int> _Socket;
        std::vector<int> _CliSocket;
        // std::vector<class Client> _Clients;
        struct sockaddr_in _sockaddr;
        int start_server();
        void close_server();
        void start_listen(int i);



    
};