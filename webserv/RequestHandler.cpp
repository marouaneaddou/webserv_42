#include "RequestHandler.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <cstdlib>
#include <string>

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
    // response->setStatus(413);
    // return (EXIT_FAILURE);

    return(EXIT_SUCCESS);
}

bool RequestHandler::req_uri_location(const Request* request, Response* response)
{
    std::string url = request->getURL();
    std::string path;
    std::size_t query_pos = url.find("?");
    if (query_pos != std::string::npos)
        path = url.substr(0, query_pos);
    else
        path = url;

    //if path doesnt match the location in the server location response->setStatus(404); return(EXIT_FAILURE);
    

    return (EXIT_SUCCESS);
}

bool RequestHandler::is_location_have_redirection(const Request* request, Response* response)
{
    //see if there is rewrite to that request url in location
    //if location have redirection response->setHeader("Location", "new_URL")
    // response->setStatus(301); 
    //response->setHeader("Content-Length", 0);
    //return(EXIT_FAILURE);

    return (EXIT_SUCCESS);
}

bool RequestHandler::is_method_allowed_in_location(const Request* request, Response* response)
{
    //[SUBJECT]Define a list of accepted HTTP methods for the route
    //if method doesnt exist in the list of accepted methods response->setStatus(405); return(EXIT_FAILURE);
    return (EXIT_SUCCESS);
}

bool RequestHandler::check_requested_method(const Request* request, Response* response)
{
    if (request->getMethod() == "GET")
    {
        if (!get_requested_ressource(request, response)) {return (EXIT_FAILURE);}
        get_ressource_type(request, response);
        if (_pathType == "DIR")
        {
            if (_path[_path.length() - 1] != '/')
            {
                response->setHeader("Location", _path + '/');
                response->setHeader("Content-Length", 0);
                response->setStatus(301);
                return (EXIT_FAILURE);
            }
            if (!is_dir_has_index_files(request, response))
            {
                //check autoindex(config) ON/OFF :

                //if its OFF 
                    // response->setStatus(403)

                //if ON generate a directory listing and response->setBody(content)
                    // response->setHeaders("Content-Type", "example: text/html; charset=utf-8");
                    // response->setHeaders("Content-Type", "size of resp body in bytes")
                    //response->setStatus(200);
                    //return(EXIT_SUCCESS);
            }
        }
        if (if_location_has_cgi(request, response))
        {
            //run cgi
            //code depend on cgi
            //response->setStatus(200);
        }
        else {
            //return requested file
            //response->setStatus(200);
        }

    }
    else if (request->getMethod() == "POST") {
    
    }
    else if (request->getMethod() == "DELETE") {
    
    }
    return (EXIT_SUCCESS);
}

bool RequestHandler::get_requested_ressource(const Request* request, Response* response)
{
    //if requested ressource not found in root in the conf file response->setStatus(404); return(EXIT_FAILURE);
    std::string url = request->getURL();
    std::size_t query_pos = url.find("?");
    if (query_pos != std::string::npos)
        _path = url.substr(0, query_pos);
    else
        _path = url;
    return (EXIT_SUCCESS);
}

void RequestHandler::get_ressource_type(const Request* request, Response* response)
{
    struct stat fileInfo;
    int result = stat(_path.c_str(), &fileInfo);
    if (S_ISDIR(fileInfo.st_mode))
       _pathType = "DIR";
    else if (S_ISREG(fileInfo.st_mode))
        _pathType = "FILE";
}

bool RequestHandler::is_dir_has_index_files(const Request* request, Response* response)
{
    std::vector<std::string> indexFiles;
    struct stat fileInfo;
    for (int i = 0; i < indexFiles.size(); i++)
    {
        std::string filePath = _path + indexFiles[i];
        if (stat(filePath.c_str(), &fileInfo) == 0 && S_ISREG(fileInfo.st_mode))
        {
            _path = _path + indexFiles[i];
            return (EXIT_SUCCESS);
        }
    }

    return (EXIT_FAILURE);
}

bool RequestHandler::if_location_has_cgi(const Request* request, Response* response)
{

    return (EXIT_SUCCESS);
}


