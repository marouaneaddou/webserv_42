#pragma once

#include "Tcp_server.hpp"
#include <iostream>
#include <vector>
#include <map>

class WebServ
{
    private:
        std::vector <std::vector<int> > _ports;
        std::vector<TCPserver *> _servers;
        std::vector<int> servers_fds;
        std::vector<int> clients_fds;
        std::map<int, client> _clients;
        struct sockaddr_in _sockaddr;

    public:
        WebServ();
        ~WebServ();
        void run_servers();
        void SetListeners();
        void set_non_blocking(int sock);
        fd_set current_Rsockets, ready_Rsockets;
        fd_set current_Wsockets, ready_Wsockets;

};