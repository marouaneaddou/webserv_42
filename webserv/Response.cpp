
#include "Response.hpp"

Response::Response()
{
    setStatus(200);
}

Response::~Response()
{
    
}

void Response::setStatus(int status)
{
    _status = status;
}

void Response::setBody(const std::string& body)
{
    _body = body;
}

std::string Response::generateResponseString() const 
{
    return (_Response);
}
