#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
# include "Response.hpp"
#include <map>
#include <cstdlib>
#include "../includes/utils.hpp"

typedef std::map<std::string, std::string>::const_iterator itHeaders;
class Request
{
    public :
        Request();
        ~Request();
        std::string getMethod() const
        {
            return _method;
        };
        void setMethod();
        std::string getURL() const;
        void setURL();
        void setHeader(std::string buffer);
        std::string getHeader() const;

        /********* MAP headers*******/

            std::map<std::string, std::string>& getHeaders() const;
            itHeaders getHeader(const char*) const;
            itHeaders getStartHeaders() const;
            itHeaders getEndHeaders() const;
            void setHeaders();

        /********* MAP headers*******/

        /********** POST ******/

            void findTypeOfPostMethod();
            void setBody(std::string &buffer);
            std::string getBody() const;
        
        /********** POST ******/
        void parse_request_line(std::string &_rawReq);
        void parse_headers_body(std::string _rawReq);
        bool isReqWellFormed(Response &response);

        /******************* PRINT DATA *****************/

            void printHeaders() const;
            void printRequest() const;

        /******************* PRINT DATA *****************/
    private:
        std::string _header;
        std::map<std::string, std::string> _headers;
        std::string _lineRequest;
        std::string _body;
        std::string _method;
        std::string _URL;
        std::string _http_version;
        std::string _rawReq;

        
};