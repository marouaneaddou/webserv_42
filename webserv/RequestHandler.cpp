#include "RequestHandler.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <cstdlib>
#include <string>
#include <variant>

RequestHandler::RequestHandler(){

}

bool RequestHandler::is_req_well_formed(const Request* request, Response* response)
{
    std::map<std::string, std::string>::iterator it1 = request->getHeaders().find("Transfer-Encoding");
    std::map<std::string, std::string>::iterator it2 = request->getHeaders().find("Content-Length");
    if (it1 != request->getHeaders().end())
    {
        if (it1->second != "chunked")
        {
            response->setStatus(501);
            return (EXIT_FAILURE);
        }
    }
    if (it1 == request->getHeaders().end() && it2 == request->getHeaders().end() && request->getMethod() == "POST")
    {
        response->setStatus(400);
        return (EXIT_FAILURE);
    }
    if (request->getURL().find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=") != std::string::npos)
    {
        response->setStatus(400);
        return (EXIT_FAILURE);
    }
    if (request->getURL().length() > 2048)
    {
        response->setStatus(414);
        return (EXIT_FAILURE);
    }
    // if(request body larger than client max size in config file)
    //response->setStatus(413);
    // return (EXIT_FAILURE);

    return(EXIT_SUCCESS);
}