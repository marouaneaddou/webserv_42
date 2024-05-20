#include "Tcp_server.hpp"
#include <sys/socket.h>

TCPserver::TCPserver(std::vector<int> port) : _Port(port)
{
    start_server();
}

TCPserver::~TCPserver()
{
    close_server();
}

void TCPserver::exit_error(const std::string err_msg)
{
    std::cout << "ERROR: " << err_msg << std::endl;
    exit (1);
}

int TCPserver::start_server()
{
    int ssocket;
    int optval = 1;
    for (int i = 0; i < _Port.size(); i++)
    {
        if ((ssocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            exit_error("cannot create socket");
        
        setsockopt(ssocket, SOL_SOCKET, SO_REUSEADDR,  &optval, sizeof(optval));
        _Socket.push_back(ssocket);
    }
    for (int i = 0; i < _Socket.size(); i++)
    {
        memset(&_sockaddr, '0', sizeof(_sockaddr));
        _sockaddr.sin_family = AF_INET;
        _sockaddr.sin_port = htons(_Port[i]);
        _sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        if (bind(_Socket[i], (struct sockaddr *)&_sockaddr, sizeof(_sockaddr)) < 0)
        {
            exit_error("Cannot connect socket to address");
            close(_Socket[i]);
        }
        start_listen(i);
    }
    return (0);
}

void TCPserver::start_listen(int i)
{
    if (listen(_Socket[i], BACKLOGS) < 0)
        exit_error("Socket listening failed");
    set_non_blocking(_Socket[i]);
}

void TCPserver::close_server()
{
    for (int i = 0; i < _Socket.size(); i++)
        close(_Socket[i]);
}

 std::vector<int> TCPserver::getSocket()  {return (_Socket);}
//  std::vector<class Client> TCPserver::getClients() {return (_Clients);}

void TCPserver::set_non_blocking(int sock) {
    int flags = fcntl(sock, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        exit(EXIT_FAILURE);
    }
    if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL");
        exit(EXIT_FAILURE);
    }
}