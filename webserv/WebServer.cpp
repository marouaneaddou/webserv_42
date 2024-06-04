#include "Webserver.hpp"
#include <cstdlib>
#include <iostream>
#include <map>
#include <sys/fcntl.h>
#include <unistd.h>
#include <utility>
#include <vector>
#include "../includes/servers.hpp"

WebServ::WebServ(){}

WebServ::~WebServ()
{
    for (int i = 0; i < _servers.size(); i++)
        delete _servers[i];
}

void WebServ::run_servers(std::vector<Servers> Confs)
{
    int pos;
    int find;
    for (int i = 0; i <  Confs.size(); i++)
        _servers.push_back(new TCPserver(Confs[i]));
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
                    int new_socket = 0;
                    socklen_t _sockaddr_len = sizeof(_sockaddr);
                    if ((new_socket = accept(*it, (sockaddr *)&_sockaddr,  &_sockaddr_len)) < 0)
                        perror("Server failed to accept incoming connection");
                    set_non_blocking(new_socket);
                    std::cout << new_socket << std::endl;
                    _clients.insert(std::make_pair(new_socket, new Client(new_socket)));
                    Servers server = getConf(*it, Confs);
                    _clients[new_socket]->setConf(server);
                    // clients_fds.push_back(new_socket);
                    FD_SET(new_socket, &current_Rsockets);
                    printf("New connection accepted.\n");
                }
                else
                {
                    read_request(idx);
                    start_parsing(idx);
                    if ( _clients[idx]->_request.getHeader("Transfer-Encoding")  != _clients[idx]->_request.getEndHeaders())
                    {
                        if (_buffer.find("0\r\n\r\n") != -1)
                        {
                            _clients[idx]->_request.findTypeOfPostMethod();
                            _clients[idx]->_request.setBody(_buffer);

                            _clients[idx]->_request.parceBody();
                            _clients[idx]->_request.printRequest();

                                /*************** Function Run Cgi Login && Register *****************/

                                // Cgi cgi(_clients[idx]->_request);
                                // _clients[idx]->runCgiLoginRegister(cgi);

                                /*************** Function Run Cgi Login && Register *****************/

                            std::cout << "here" << std::endl;
                            _buffer.clear();
                            FD_CLR(idx, &current_Rsockets);
                            FD_SET(idx, &current_Wsockets);
                        }
                    }
                    else if (_clients[idx]->_request.getHeader("Content-Length")  != _clients[idx]->_request.getEndHeaders())
                    {
                        if (_buffer.size() == stoi(_clients[idx]->_request.getHeader("Content-Length")->second))
                        {
                            _clients[idx]->_request.findTypeOfPostMethod();
                            _clients[idx]->_request.setBody(_buffer);

                            _clients[idx]->_request.parceBody();

                            _clients[idx]->_request.printRequest();

                                /*************** Function Run Cgi Login && Register *****************/

                                // Cgi cgi(_clients[idx]->_request);
                                // _clients[idx]->runCgiLoginRegister(cgi);

                                /*************** Function Run Cgi Login && Register *****************/
                                
                            _buffer.clear();
                            FD_CLR(idx, &current_Rsockets);
                            FD_SET(idx, &current_Wsockets);
                        }
                    }

                }
            }
            else if (FD_ISSET(idx, &ready_Wsockets))
            {
                if (_clients.at(idx)->_response.getStatus() == 200)
                {
                    RequestHandler* handler = createHandler(_clients.at(idx)->_request);
                    handler->handleRequest(_clients.at(idx));
                }
                // char httpResponse[] = "HTTP/1.1 200 OK\r\n"
                //      "Date: Mon, 20 May 2024 12:34:56 GMT\r\n"
                //      "Server: Apache/2.4.41 (Ubuntu)\r\n"
                //      "Content-Type: text/plain; charset=UTF-8\r\n"
                //      "Content-Length: 13\r\n"
                //      "\r\n"
                //      "Hello, World!";
                // int nbyte = send(idx, httpResponse, strlen(httpResponse), 0);
                FD_CLR(idx, &current_Wsockets);
                close(idx);
                itClient it = _clients.find(idx);
                _clients.erase(it);
            }
        }
    }
}

void WebServ::read_request(int fd_R)
{
    if ((_nbytes = recv(fd_R, _buf, 25000, 0)) <= 0)
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

}

void WebServ::start_parsing(int fd_R)
{
    if (_clients.at(fd_R)->getCheck() == false &&_buffer.find("\r\n\r\n") != -1)
    {
        int findPos = _buffer.find("\r\n");
        _clients.at(fd_R)->setCheck();

        _firstline = _buffer.substr(0, findPos);
        _clients.at(fd_R)->_request.parse_request_line(_firstline);
        _buffer = _buffer.substr(findPos + 2);
        if (_clients.at(fd_R)->_request.getMethod() != "POST")
        {
            _clients.at(fd_R)->_request.setHeader(_buffer);
            _clients.at(fd_R)->_request.printRequest();
            _clients[fd_R]->_request.isReqWellFormed(_clients[fd_R]->getResponse());
            FD_CLR(fd_R, &current_Rsockets);
            
            FD_SET(fd_R, &current_Wsockets);
            _buffer.clear();
        }
        else
        {
            findPos = _buffer.find("\r\n\r\n");
            _clients.at(fd_R)->_request.setHeader(_buffer.substr(0, findPos + 2));
            _clients[fd_R]->_request.isReqWellFormed(_clients[fd_R]->getResponse());
            _buffer = _buffer.substr(findPos + 4);
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

void WebServ::set_non_blocking(int sock) 
{
    if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1) 
    {
        perror("fcntl F_SETFL");
        exit(EXIT_FAILURE);
    }
}


RequestHandler* WebServ::createHandler(Request &request) 
{
    if (isPHPCGIRequest(request.getURL()))
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

Servers WebServ::getConf(int fd, std::vector<Servers> Confs)
{
    for (int i = 0; i < _servers.size(); i++)
    {
        for (int j = 0; j < _servers[i]->getSocket().size(); j++) 
        {
            if (_servers[i]->getSocket()[j] == fd)
            {
                return Confs[i];
            }
        }
    }
    return (Confs[0]);
}
