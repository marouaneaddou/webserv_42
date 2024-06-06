
#include "Response.hpp"
#include <string>
#include <sys/socket.h>

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
    std::cout << "response : " << _Response << std::endl;
}


void Response::Send(int cli_fd)
{
    generateResponseString();
    int nbyte = send(cli_fd, _Response.c_str(), strlen(_Response.c_str()), 0);
}
