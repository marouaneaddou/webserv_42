#include "HTTPRequest.hpp"
#include <cstdlib>

HTTPRequest::HTTPRequest()
{

}
HTTPRequest::HTTPRequest(const std::string &raw_req) : _rawReq(raw_req)
{
    parse_request_line(_rawReq);
    parse_headers_body(_rawReq);
}

HTTPRequest::~HTTPRequest(){};

void HTTPRequest::parse_request_line(std::string &_rawReq)
{
    _lineRequest = _rawReq;
    std::string oneline;
    std::istringstream iss(_rawReq);
    std::getline(iss, oneline);
    // std::vector<std::string> tokens = split(oneline, ' ');
    std::string dele = " ";
    std::vector<std::string> tokens = Utils::split(oneline, dele);
    _method = tokens[0];
    _URL = tokens[1];
    std::cout << tokens.size() << std::endl;
    http_version = tokens[2];
}

// std::vector<std::string> HTTPRequest::split(const std::string& input, char delimiter) {
//     std::vector<std::string> tokens;
//     std::string token;
//     for (size_t i = 0; i < input.size(); ++i) {
//         if (input[i] != delimiter) {
//             token += input[i];
//         } else {
//             tokens.push_back(token);
//             token.clear();
//         }
//     }
//     return tokens;
// }

void HTTPRequest::parse_headers_body(std::string _rawReq)
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

void HTTPRequest::setHeader(std::string &buffer)
{
    _header = buffer;
}

std::string HTTPRequest::getHeader() const
{
    return _header;
}

// int HTTPRequest::check_req_format()
// {
//     std::map<std::string, std::string>::iterator it1 = _headers.find("Transfer-Encoding");
//     std::map<std::string, std::string>::iterator it2 = _headers.find("Content-Length");
//     int found = getURL().find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=");
//     if (it1 != _headers.end())
//     {
//         if (it1->second != "chunked")
//         {
//             _status = 501;
//             _statusMsg = "Not Implemented";
//             return (EXIT_FAILURE);
//         }
//     }
//     else if (it2 != _headers.end() || found != std::string::npos)
//     {
//         _status = 400;
//         _statusMsg = "Bad Request";
//         return(EXIT_FAILURE);
//     }

//     return (EXIT_SUCCESS);
// }