#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <cstdlib>

class HTTPRequest
{
    public :
        HTTPRequest(const std::string &raw_req);
        ~HTTPRequest();
        std::string getMethod() const;
        std::string getURL() const;
        std::string getBody() const;
        std::string getStatus() const;
        std::map<std::string, std::string> getHeaders() const;
        void parse_request_line(std::string _rawReq);
        void parse_headers_body(std::string _rawReq);
        // int check_req_format();
        std::vector<std::string> split(const std::string& input, char delimiter);

    private:
        std::map<std::string, std::string> _headers;
        std::string _body;
        std::string _Method;
        std::string _URL;
        std::string http_version;
        std::string _rawReq;
        int _status;
        std::string _statusMsg;

        
};