#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include "includes_util.hpp"
#include "servers.hpp"
#include "Tcp_server.hpp"
# include "Client.hpp"
#include "RequestHandler.hpp"
#include "StaticFileHandler.hpp"

typedef  std::map<int, Client *>::iterator itClient;

class WebServ
{
    private:
        int max_fd;
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
        RequestHandler* handler;
        int _exactLocation;
        std::string _path;
    public:
        WebServ();
        ~WebServ();
        
        void run_servers(std::map<int, std::vector<Servers> > grouped, std::vector<Servers> confs);
        // void run_servers(std::vector<std::vector<Servers> > confs);
        Servers getConf(int fd, std::map<int, std::vector<Servers> > conf, std::string host);
        // Servers getConf(int fd, std::vector<std::vector<Servers> > conf);
        void SetListeners();
        int set_non_blocking(int sock);
        void read_request(int fd_R);
        void start_parsing(int fd_R, std::map<int, std::vector<Servers> > grouped);
        RequestHandler* createHandler(Request &request, int &exactLocation, std::string &path);
        bool isPHPCGIRequest(std::string URL);

        void req_uri_location(Client* cli);
        bool is_location_have_redirection(Client* cli);
        void is_method_allowed_in_location(Client* cli);

        void setExactIndexLocation(int &index);
        int getExactIndexLocation() const;



        fd_set current_Rsockets, ready_Rsockets;
        fd_set current_Wsockets, ready_Wsockets;

};
# endif