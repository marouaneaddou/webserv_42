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
    int pos;
    int find;
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
                    read_request(idx);
                    start_parsing(idx);
                }
            }
            else if (FD_ISSET(idx, &ready_Wsockets))
            {
                RequestHandler* handler = createHandler(_clients.at(idx)->_request);
                // handler->handleRequest(_clients.at(idx)->_request, _clients.at(idx)->_response);
                char httpResponse[] = "HTTP/1.1 200 OK\r\n"
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

void WebServ::read_request(int fd_R)
{
    if ((_nbytes = recv(fd_R, _buf, sizeof(_buf), 0)) <= 0)
    {
        // got error or connection closed by client
        if (_nbytes == 0)
            printf("selectserver: socket %d hung up\n", fd_R);
        else
            perror("read");
        close(fd_R);
        FD_CLR(fd_R, &current_Rsockets);
        // continue;
    }
    if (_nbytes < sizeof(_buf))
        _buf[_nbytes] = '\0';
    _buffer.append(_buf, _nbytes);
    // if (_nbytes == 0)
    //     std::cout << "kamalt 9raya" << std::endl;
    // std::cout << "nbyt " << _nbytes << std::endl;
    // if (_buffer.find("0") != -1)
    //     std::cout << _buffer << std::endl;
    // std::cout << "*****************************\n";
}

void WebServ::start_parsing(int fd_R)
{
    if (_buffer.find("\r\n\r\n") != -1)
    {
        _firstline = _buffer.substr(0, _buffer.find("\r\n"));
        _clients.at(fd_R)->_request->parse_request_line(_firstline);
        _buffer = _buffer.substr(_buffer.find("\r\n"));
        if (_clients.at(fd_R)->_request->getMethod() != "POST")
        {
            _clients.at(fd_R)->_request->setHeader(_buffer);
            _buffer.clear();
            FD_CLR(fd_R, &current_Rsockets);
            FD_SET(fd_R, &current_Wsockets);
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

RequestHandler* WebServ::createHandler(Request* request) 
{
        if (isPHPCGIRequest(request->getURL())) 
        {
            return new PhpCgiHandler();
        }
        else 
        {
            return new StaticFileHandler();
        }
}

bool WebServ::isPHPCGIRequest(const std::string url) 
{
    if (url.find(".php") != std::string::npos) // this check is not 100% done!!!!
    {
        return true;
    }
    return false;
}
