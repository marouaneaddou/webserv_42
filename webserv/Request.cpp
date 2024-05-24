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
    size_t separator_pos = 0;
    std::string line;
    while ((pos = _header.find("\r\n")) != std::string::npos) {

        line = _header.substr(0, pos);
        separator_pos = line.find(": ");
        if (separator_pos != std::string::npos)
            _headers[line.substr(0, separator_pos)] = line.substr(separator_pos + 2);
        _header.erase(0, pos + 2);
    }
    for (auto ut : _headers)
    {
        std::cout << ut.first << "============="  <<  ut.second <<std::endl;
    }
}

itHeaders Request::getHeader(const char* key) const
{
    return _headers.find(key);
}

itHeaders Request::getStartHeaders() const
{
    return _headers.begin();
}

itHeaders Request::getEndHeaders() const
{
    return _headers.end();
}

void Request::findBoundry()
{
    itHeaders it = _headers.find("Content-Type");
    std::string boundry = it->second.substr(it->second.find("=") + 1);
    boundry += "--";
    _headers["boundary"] = boundry;
    std::cout << boundry << std::endl;
    // for (auto ut : _headers)
    // {
    //     std::cout << ut.first << "============="  <<  ut.second <<std::endl;
    // }
}

void Request::setBody(std::string &buffer)
{
    _body = buffer;
}

std::string Request::getBody() const
{
    return _body;
}

bool Request::isReqWellFormed(Response &response)
{
    int status;
    if (_method == "POST")
    {
        itHeaders it1 = _headers.find("Transfer-Encoding");
        itHeaders it2 = _headers.find("Content-Length");
        if (it1 != _headers.end())
        {
            if (it1->second != "chunked")
            {
                status = 501;
                response.setStatus(status);
                return (EXIT_FAILURE);
            }
        }
        if ( _method == "POST" && it1 == _headers.end() && it2 == _headers.end() )
        {
            status = 400;
            response.setStatus(status);
            std::cout << response.getStatus() << std::endl;
            return (EXIT_FAILURE);
        }
    }
    else if (_URL.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=") != std::string::npos)
    {
        status = 400;
        response.setStatus(status);
        return (EXIT_FAILURE);
    }
    else if (_URL.size() > 2048)
    {
        status = 414;
        response.setStatus(status);
        return (EXIT_FAILURE);
    }
    return(EXIT_SUCCESS);
}