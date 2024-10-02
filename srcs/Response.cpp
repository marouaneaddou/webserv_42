
#include "../includes/Response.hpp"

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

void Response::setAppendBody(const char *buffer)
{
    _body.append(buffer);
}

std::string Response::getBody() const
{
    return _body;
}


    /*************** Body ************/

void Response::generateHeaderResponse() 
{
    _Response = "HTTP/1.1 " + std::to_string(getStatus()) + ' ' + getStatusMsg() + "\r\n";
    std::map<std::string, std::string>::iterator it =  _headers.begin();
    while (it != _headers.end())
    {
        _Response += it->first + ": " + it->second;
        if (++it != _headers.end()) {
            _Response += "\r\n";
        }
    }
    _Response += "\r\n\r\n";
    _Response += _body;
    std::cout << "\"" <<_Response <<"\""<< std::endl;
}


void Response::Send(int cli_fd)
{
    signal(SIGPIPE, SIG_IGN);
    int read;
    if (_Response.size() > 1000000) read = 1000000;
    else read = _Response.size();
    int nbyte = send(cli_fd, _Response.data(), read, 0);
    _Response.erase(0, nbyte);
}
