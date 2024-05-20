#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "Tcp_server.hpp"
#include <iostream>
#include <vector>
#include <map>
# include "Client.hpp"

class WebServ
{
    private:
        std::vector <std::vector<int> > _ports;
        std::vector<TCPserver *> _servers;
        std::vector<int> servers_fds;
        std::vector<int> clients_fds;
        std::map<int, Client*> _clients;
        struct sockaddr_in _sockaddr;
        // std::map<int , int> check;
        // bool check;
    public:
        WebServ();
        ~WebServ();
        void run_servers();
        void SetListeners();
        void set_non_blocking(int sock);
        void selectTypeOfMethod(std::string &buffer, int &fdSocket);
        fd_set current_Rsockets, ready_Rsockets;
        fd_set current_Wsockets, ready_Wsockets;

};
# endif