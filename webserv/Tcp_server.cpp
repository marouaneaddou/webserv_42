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
    for (int i = 0; i < _Port.size(); i++)
    {
        if ((ssocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
            exit_error("cannot create socket");
        _Socket.push_back(ssocket);
    }
    for (int i = 0; i < _Socket.size(); i++)
    {
        memset(&_sockaddr, '0', sizeof(_sockaddr));
        _sockaddr.sin_family = AF_INET;
        _sockaddr.sin_port = htons(_Port[i]);
        _sockaddr.sin_addr.s_addr = inet_addr("10.11.8.2");
        if (bind(_Socket[i], (struct sockaddr *)&_sockaddr, sizeof(_sockaddr)) < 0)
            exit_error("Cannot connect socket to address");
        start_listen(i);
        FD_ZERO(&current_Rsockets);
        FD_SET(_Socket[i], &current_Rsockets);
        FD_ZERO(&current_Wsockets);
        FD_SET(_Socket[i], &current_Wsockets);
    }
    return (0);

}

void TCPserver::start_listen(int i)
{
    if (listen(_Socket[i], BACKLOGS) < 0)
        exit_error("Socket listening failed");
}

int TCPserver::acceptConnection(int socket)
{
    int cli_sock = 0;
    socklen_t _sockaddr_len = sizeof(_sockaddr);
        if ((cli_sock = accept(socket, (sockaddr *)&_sockaddr,  &_sockaddr_len)) < 0)
            perror("Server failed to accept incoming connection");
        // _Clients.push_back(Client(cli_sock, _sockaddr));
        // _CliSocket.push_back(cli_sock);
    return (cli_sock);
}

void TCPserver::close_server()
{
    for (int i = 0; i < _Socket.size(); i++)
        close(_Socket[i]);
    // for (int i = 0; i < _CliSocket.size(); i++)
    //     close(_CliSocket[i]);
}

 std::vector<int> TCPserver::getSocket()  {return (_Socket);}
 std::vector<int> TCPserver::getCliSocket()  {return (_CliSocket);}
//  std::vector<class Client> TCPserver::getClients() {return (_Clients);}

