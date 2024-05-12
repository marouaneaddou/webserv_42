#pragma once

#include "Tcp_server.hpp"
#include <iostream>
#include <vector>

class WebServ
{
    private:
        std::vector <std::vector<int> > _ports;
        std::vector<TCPserver *> _servers;
        std::vector<int> servers_fds;
        std::vector<int> clients_fds;
        struct sockaddr_in _sockaddr;

    public:
        WebServ();
        ~WebServ();
        void run_servers();
        void SetListeners();
        fd_set current_Rsockets, ready_Rsockets;
        fd_set current_Wsockets, ready_Wsockets;

};