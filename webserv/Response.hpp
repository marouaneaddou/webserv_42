#pragma once

#include <iostream>
#include <map>
class Response {
public:
    Response();
    ~Response();
    void setStatus(int status);
    void setHeader(const std::string& key, const std::string& value);
    void setBody(const std::string& body);
    std::string generateResponseString() const;
    // std::string toString() const; // Convert the response to a string for sending.
private:
    int _status;
    std::string _Response;
    std::map<std::string, std::string> _headers;
    std::string _body;
};
