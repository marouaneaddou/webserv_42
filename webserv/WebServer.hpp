#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "Tcp_server.hpp"

#include <iostream>
#include <vector>
#include <map>
# include "Client.hpp"
#include "RequestHandler.hpp"
#include "StaticFileHandler.hpp"
#include "PhpCgiHandler.hpp"
#include "../includes/servers.hpp"
# include <algorithm>
typedef  std::map<int, Client *>::iterator itClient;

class WebServ
{
    private:
        int _nbytes;
        char _buf[250000];
        std::string _buffer;
        std::string _firstline;
        std::vector <std::vector<int> > _ports;
        std::vector<TCPserver *> _servers;
        std::vector<int> servers_fds;
        std::vector<int> clients_fds;
        std::map<int, Client*> _clients;
        std::map<int, Servers> _myconf;
        struct sockaddr_in _sockaddr;
        // std::map<int , int> check;
        // bool check;
        RequestHandler* handler;
    public:
        WebServ();
        ~WebServ();
        
        void run_servers(std::vector<Servers> Confs);
        Servers getConf(int fd, std::vector<Servers> Confs);
        void SetListeners();
        void set_non_blocking(int sock);
        void read_request(int fd_R);
        void start_parsing(int fd_R);
        RequestHandler* createHandler(Request &request);
        bool isPHPCGIRequest(std::string URL);
        fd_set current_Rsockets, ready_Rsockets;
        fd_set current_Wsockets, ready_Wsockets;

};
# endif