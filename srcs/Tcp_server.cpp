#include "../includes/servers.hpp"
#include "../includes/Tcp_server.hpp"


TCPserver::TCPserver(Servers &server)
{
    start_server(server);
}

TCPserver::~TCPserver()
{
    close_server();
}

int TCPserver::start_server(Servers &server)
{
    int ssocket;
    int optval = 1;
    int status;
    struct addrinfo hints, *res;
    for (unsigned int i = 0; i < server.get_ports().size(); i++)
    {
        memset(&hints, 0, sizeof hints);
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        if ((status = getaddrinfo(server.get_host().c_str(), std::to_string(server.get_ports()[i]).c_str(), &hints, &res)) != 0) {
            std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
            exit(1);
        }
        if ((ssocket = socket(res->ai_family, res->ai_socktype, 0)) < 0)
        {
            perror("error");
            exit(1);
        }
        setsockopt(ssocket, SOL_SOCKET, SO_REUSEADDR,  &optval, sizeof(optval));
        _Socket.push_back(ssocket);
        if (bind(ssocket, res->ai_addr, res->ai_addrlen) < 0)
        {
            std::cerr << "Error: " << strerror(errno) << std::endl;
            close(ssocket);
            exit(1);
        }
        if (listen(ssocket, BACKLOGS) < 0)
        {
            std::cerr << "Error: " << strerror(errno) << std::endl;
            close(ssocket);
            exit(1);
        }
        set_non_blocking(ssocket);
    }
    return (0);
}

void TCPserver::close_server()
{
    for (unsigned int i = 0; i < _Socket.size(); i++)
        close(_Socket[i]);
}

std::vector<int> TCPserver::getSocket()  {return (_Socket);}

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