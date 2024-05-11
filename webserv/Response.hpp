#pragma once

#include <iostream>
#include <map>
class Response {
public:
    Response();
    void setStatus(int statusCode);
    void setHeader(const std::string& key, const std::string& value);
    void setBody(const std::string& body);
    std::string toString() const; // Convert the response to a string for sending.
private:
    int statusCode;
    std::map<std::string, std::string> headers;
    std::string body;
};
