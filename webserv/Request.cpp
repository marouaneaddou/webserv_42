#include "Request.hpp"
#include <cstdlib>


Request::Request()
{
}

Request::~Request(){};

void Request::parse_request_line(std::string &_rawReq)
{
    _lineRequest = _rawReq;
    std::string oneline;
    std::istringstream iss(_rawReq);
    std::getline(iss, oneline);
    std::string dele = " ";
    std::vector<std::string> tokens = Utils::split(oneline, dele);
    _method = tokens[0];
    _URL = tokens[1];
    std::cout << tokens.size() << std::endl;
    http_version = tokens[2];
}


void Request::parse_headers_body(std::string _rawReq)
{
    std::istringstream iss(_rawReq);
    std::string line;
    while (std::getline(iss, line)) /////?
    {
        if (line == "\r")
            break;
        std::string::size_type pos = line.find(':');
        if (pos != std::string::npos)
        {
           _headers.insert(std::pair<std::string, std::string>(line.substr(0, pos - 1),
                line.substr(pos + 1, line.length())));
        }
    }
    while (std::getline(iss, line))
    {
       if (line != "\r")
       {
           _body.append(line);
           _body.append("\n");
       }
    }
}

void Request::setHeader(std::string &buffer)
{
    _header = buffer;
}

std::string Request::getHeader() const
{
    return _header;
}
