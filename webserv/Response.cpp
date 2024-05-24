
#include "Response.hpp"

Response::Response()
{
    int status = 200;
    setStatus(status);
}

Response::~Response()
{
    
}

    /*************** Status ************/

void Response::setStatus(int &status)
{
    _status = status;
}

int Response::getStatus() const
{
    return _status;
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
std::string Response::generateResponseString() const 
{
    return (_Response);
}
