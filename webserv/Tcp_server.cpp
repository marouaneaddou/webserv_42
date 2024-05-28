#include "Tcp_server.hpp"
#include <string>
#include <sys/socket.h>


TCPserver::TCPserver(Servers &server)
{
    start_server(server);
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

int TCPserver::start_server(Servers &server)
{
    int ssocket;
    int optval = 1;
    int status;
    struct addrinfo hints, *res;


    for (int i = 0; i < server.ports.size(); i++)
    {
        memset(&hints, 0, sizeof hints); // make sure the struct is empty
        hints.ai_family = AF_UNSPEC;     // don't care IPv4 or IPv6
        hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
        hints.ai_flags = AI_PASSIVE;     // fill in my IP for me
        if ((status = getaddrinfo(server.host.c_str(), std::to_string(server.ports[i]).c_str(), &hints, &res)) != 0) {
            fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
            exit(1);
        }
        if ((ssocket = socket(res->ai_family, res->ai_socktype, 0)) < 0)
            exit_error("cannot create socket");
        setsockopt(ssocket, SOL_SOCKET, SO_REUSEADDR,  &optval, sizeof(optval));
        _Socket.push_back(ssocket);
    }
    for (int i = 0; i < _Socket.size(); i++)
    {   
        if (bind(_Socket[i], res->ai_addr, res->ai_addrlen) < 0)
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