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

void Request::setHeader(std::string buffer)
{
    _header = buffer;
    setHeaders();
}

std::string Request::getHeader() const
{
    return _header;
}

std::string Request::getURL() const
{
    return _URL;
}

void Request::setHeaders()
{
    size_t pos = 0;
    while ((pos = _header.find("\r\n")) != std::string::npos) {
        std::string line = _header.substr(0, pos);
        size_t separator_pos = line.find(": ");
        if (separator_pos != std::string::npos) {
            std::string key = line.substr(0, separator_pos);
            std::string value = line.substr(separator_pos + 2);
            _headers[key] = value;
        }
        _header.erase(0, pos + 2);
    }
    for (auto ut : _headers)
    {
        std::cout << ut.first << " "  <<  ut.second <<std::endl;
    }
    // std::cout << _header << std::endl;
}