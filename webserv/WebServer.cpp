#include "Webserver.hpp"
#include <cstdlib>
#include <map>
#include <sys/fcntl.h>
#include <unistd.h>
#include <vector>

WebServ::WebServ()
{
    // fill infos from config class;
    std::vector<int> port;
    port.push_back(4223);
    _ports.push_back(port);

}

WebServ::~WebServ()
{
    for (int i = 0; i < _servers.size(); i++)
        delete _servers[i];
}

void WebServ::run_servers()
{
    int nbytes;
    int pos;
    std::string buffer;
    std::string line;
    // bool check = false;
    char buf[20];
    for (int i = 0; i < _ports.size(); i++)
    {
        _servers.push_back(new TCPserver(_ports[i]));
    }
    SetListeners();
    while (true)
    {
        ready_Rsockets = current_Rsockets;
        ready_Wsockets = current_Wsockets;
        if (select(FD_SETSIZE, &ready_Rsockets, &ready_Wsockets, NULL, NULL) < 0)
            std::cerr << "Error in select(): " << strerror(errno) << std::endl;
        for (int idx = 0; idx < FD_SETSIZE; idx++)
        {
            if (FD_ISSET(idx, &ready_Rsockets))
            {
                std::vector<int>::iterator it = std::find(servers_fds.begin(), servers_fds.end(), idx);
                if (it != servers_fds.end() /*&& servers[i]->getClients().size() < MAX_CONNECTIONS*/)
                {
                    std::cout << *it << std::endl;
                    int new_socket = 0;
                    socklen_t _sockaddr_len = sizeof(_sockaddr);
                    if ((new_socket = accept(*it, (sockaddr *)&_sockaddr,  &_sockaddr_len)) < 0)
                        perror("Server failed to accept incoming connection");
                    set_non_blocking(new_socket);
                    std::cout << new_socket << std::endl;
                    _clients.insert(std::make_pair(new_socket, new Client(new_socket)));
                    // clients_fds.push_back(new_socket);
                    FD_SET(new_socket, &current_Rsockets);
                    // FD_SET(new_socket, &current_Wsockets);
                    printf("New connection accepted.\n");
                }
                else
                {
                    if ((nbytes = recv(idx, buf, sizeof buf, 0)) <= 0)
                    {
                        // got error or connection closed by client
                        if (nbytes == 0)
                            printf("selectserver: socket %d hung up\n", idx);
                        else
                            perror("read");
                        close(idx);
                        FD_CLR(idx, &current_Rsockets);
                        continue;
                    }
                    buf[nbytes] = '\0';
                    buffer.append(buf, nbytes);
                    std::cout << buffer << std::endl;
                    std::cout << "*****************************\n";
                    if (buffer.find("\r\n\r\n") != -1)
                    {
                        line = buffer.substr(0, buffer.find("\r\n"));
                        _clients.at(idx)->request->parse_request_line(line);
                        buffer = buffer.substr(buffer.find("\r\n"));
                        if (_clients.at(idx)->request->getMethod() != "POST")
                        {
                            _clients.at(idx)->request->setHeader(buffer);
                            buffer.clear();
                            FD_CLR(idx, &current_Rsockets);
                            FD_SET(idx, &current_Wsockets);
                        }
                        else 
                        {
                            // line = buffer.substr(0, buffer.find("\r\n\r\n"));
                            // buffer = buffer.substr(buffer.find("\r\n\r\n"));
                            // std::cout << buffer << std::endl;
                            // std::cout << "SGV" << std::endl;
                        }
                        // selectTypeOfMethod(buffer, idx);
                    }
                    // std::cout << buffer << std::endl;
                }
            }
            else if (FD_ISSET(idx, &ready_Wsockets))
            {
                char* httpResponse = "HTTP/1.1 200 OK\r\n"
                     "Date: Mon, 20 May 2024 12:34:56 GMT\r\n"
                     "Server: Apache/2.4.41 (Ubuntu)\r\n"
                     "Content-Type: text/plain; charset=UTF-8\r\n"
                     "Content-Length: 13\r\n"
                     "\r\n"
                     "Hello, World!";
                int nbyte = send(idx, httpResponse, strlen(httpResponse), 0);
                FD_CLR(idx, &current_Wsockets);
                close(idx);
                // FD_SET(idx, &current_Rsockets);
            }
        }
    }
}



void WebServ::SetListeners()
{
    FD_ZERO(&current_Rsockets);
    FD_ZERO(&current_Wsockets);
    for (int i = 0; i < _servers.size(); i++)
    {
        for (int j = 0; j < _servers[i]->getSocket().size(); j++)
        {
            servers_fds.push_back(_servers[i]->getSocket()[j]);
            FD_SET(_servers[i]->getSocket()[j], &current_Rsockets);
        }
    }
}

void WebServ::set_non_blocking(int sock) {
    if (fcntl(sock, F_SETFL | O_NONBLOCK) == -1) {
        perror("fcntl F_SETFL");
        exit(EXIT_FAILURE);
    }
}

void WebServ::selectTypeOfMethod(std::string &buffer, int &fd)
{
    int pos = buffer.find("\r\n");
    // _clients.at(fd)->request->
}
