#pragma once

#include <unistd.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
// #include <sys/_select.h>
#include <cstring>
#include <vector>
#include <fcntl.h>
#include <iostream>



#define MAX_CONNECTIONS 2048
#define BACKLOGS 512
class TCPserver
{
    public:
        TCPserver(std::vector<int> port);
        ~TCPserver();
        void exit_error(const std::string err_msg);
        std::vector<int> getSocket() ;
        std::vector<int> getCliSocket() ;
        // const & Config getConfig();

    private:
        int _backlogs;
        std::vector<int> _Port;
        std::vector<int> _Socket;
        struct sockaddr_in _sockaddr;
        int start_server();
        void close_server();
        void start_listen(int i);
        void set_non_blocking(int sock);



    
};