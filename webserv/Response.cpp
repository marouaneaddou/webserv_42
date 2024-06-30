
#include "Response.hpp"
#include <string>
#include <sys/socket.h>
#include <cstring>

Response::Response()
{
    setStatus(200);
}

Response::~Response()
{
    
}
it_Header Response::getHeader(const std::string& key) 
{
        return _headers.find(key);
}
    /*************** Status ************/

void Response::setStatus(const int &status)
{
    _status = status;
}

void Response::setStatusMsg(const std::string& msg)
{
    this->_Msg = msg;
}

int Response::getStatus() const
{
    return _status;
}

std::string Response::getStatusMsg() const
{
    return _Msg;
}
    /*************** Status ************/

    /*************** Body ************/

void Response::setBody(const std::string& body)
{
    _body = body;
}

std::string Response::getBody() const
{
    return _body;
}


    /*************** Body ************/

void Response::generateResponseString() 
{

    _Response = "HTTP/1.1 " + std::to_string(getStatus()) + ' ' + getStatusMsg() + "\r\n";
    std::map<std::string, std::string>::iterator it =  _headers.begin();
    while (it != _headers.end())
    {
        _Response += it->first + ": " + it->second;
        if (++it != _headers.end())
            _Response += "\n";
    }
    _Response += "\r\n\r\n";
    _Response += getBody();
}


void Response::Send(int cli_fd)
{
    // generateResponseString();
    std::cout << "size ->" << _Response.size() << std::endl;
    // std::cout << _Response.c_str() << std::endl;
    int nbyte = send(cli_fd, _Response.data(), 1000000, 0);
    if (nbyte == -1)
        std::cout << "ERROR SEND\n";
    std::cout << "nbyte ->" << nbyte << std::endl;
    _Response.erase(0, nbyte);
    // std::cout << "hnaya hnaya hnaya hnaya hnaya \n";
}
