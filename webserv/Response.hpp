#pragma once

# include <iostream>
# include <map>

#include <sstream>
#include <string>
class Response {
public:
    Response();
    ~Response();
    template<typename T>
    void setHeader( std::string key,  T value)
    {
        std::ostringstream oss;
            oss << value;
        _headers[key] = oss.str();
    }
    void setStatus(const int &status);
    void setStatusMsg(const std::string &Msg);
    void setBody(const std::string& body);
    int getStatus() const;
    std::string getBody() const;
    std::string generateResponseString() const;
    // std::string toString() const; // Convert the response to a string for sending.
private:
    int _status;
    std::string _Msg;
    std::string _Response;
    std::map<std::string, std::string> _headers;
    std::string _body;
};
