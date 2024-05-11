#include "HTTPRequest.hpp"


int main()
{
    std::string httpRequest = "GET /index.html HTTP/1.1\r\n"
                          "Host: www.example.com\r\n"
                          "User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/97.0.4692.99 Safari/537.36\r\n"
                          "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3\r\n"
                          "Accept-Encoding: gzip, deflate, br\r\n"
                          "Connection: keep-alive\r\n"
                          "\r\n"
                          "This is the body of the request.";

    HTTPRequest request(httpRequest);

    

}