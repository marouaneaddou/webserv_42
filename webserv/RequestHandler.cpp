#include "RequestHandler.hpp"
#include "Request.hpp"
#include "Response.hpp"
#include <cstddef>
#include <cstdlib>
#include <string>
#include <sys/stat.h>
#include <vector>

RequestHandler::RequestHandler(){

}

// bool RequestHandler::is_req_well_formed(Client* cli)
// {
    // itHeaders it1 = cli->_request.getHeaders().find("Transfer-Encoding");
    // itHeaders it2 = cli->_request.getHeaders().find("Content-Length");
    // if (it1 != cli->_request.getEndHeaders())
    // {
    //     if (it1->second != "chunked")
    //     {
    //         cli->_response.setStatus(501);
    //         return (EXIT_FAILURE);
    //     }
    // }
    // if (it1 == cli->_request.getEndHeaders() && it2 == cli->_request.getEndHeaders() /*&& cli->_request.getMethod() == "POST"*/)
    // {
    //     cli->_response.setStatus(400);
    //     return (EXIT_FAILURE);
    // }
    // if (cli->_request.getURL().find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=") != std::string::npos)
    // {
    //     cli->_response.setStatus(400);
    //     return (EXIT_FAILURE);
    // }
    // if (cli->_request.getURL().length() > 2048)
    // {
    //     cli->_response.setStatus(414);
    //     return (EXIT_FAILURE);
    // }
    // if(request body larger than client max size in config file)
    // cli->_response.setStatus(413);
    // return (EXIT_FAILURE);

//     return(EXIT_SUCCESS);
// }

bool RequestHandler::req_uri_location(Client* cli)
{
    std::string url = cli->_request.getURL();
    std::string path;
    std::size_t query_pos = url.find("?");
    if (query_pos != std::string::npos)
        path = url.substr(0, query_pos);
    else
        path = url;

    //if path doesnt match the location in the server location cli->_response.setStatus(404); return(EXIT_FAILURE);
    

    return (EXIT_SUCCESS);
}

bool RequestHandler::is_location_have_redirection(Client* cli)
{
    //see if there is rewrite to that request url in location
    //if location have redirection cli->_response.setHeader("Location", "new_URL")
    // cli->_response.setStatus(301); 
    //cli->_response.setHeader("Content-Length", 0);
    //return(EXIT_FAILURE);

    return (EXIT_SUCCESS);
}

bool RequestHandler::is_method_allowed_in_location(Client* cli)
{
    std::vector<std::string> accepted_methods; //get it from cli->conf->accepted_methods
    for (std::vector<std::string>::const_iterator it = accepted_methods.begin(); it != accepted_methods.end(); ++it) 
    {
        if (*it == cli->_request.getMethod())  
            return (EXIT_SUCCESS);
    }
    cli->_response.setStatus(405);
    return(EXIT_FAILURE);
}

bool RequestHandler::check_requested_method(Client* cli)
{
    if (cli->_request.getMethod() == "GET")
    {
        if (!get_requested_ressource(cli)) {return (EXIT_FAILURE);}
    
        if (get_ressource_type(cli) == "DIR")
        {
            if (_path[_path.length() - 1] != '/')
            {
                cli->_response.setHeader("Location", _path + '/');
                cli->_response.setHeader("Content-Length", 0);
                cli->_response.setStatus(301);
                return (EXIT_FAILURE);
            }
            if (!is_dir_has_index_files(cli))
            {
                 
                //check autoindex(config) ON/OFF :
                

                //if its OFF 
                    // cli->_response.setStatus(403);
                    //return(EXIT_FAILURE);

                //if ON generate a directory listing and cli->_response.setBody(HTML page with the content in the directory)
                    cli->_response.setHeader("Content-Type", "text/html");
                    // cli->_response.setHeader("Content-Length", "size of resp body in bytes")
                    // cli->_response.setStatus(200);
                    // return(EXIT_SUCCESS);
            }
        }
        if (if_location_has_cgi(cli))
        {
            //run cgi
            //code depend on cgi
            // cli->_response.setStatus(200); ?
        }
        else {
            //return requested file cli->_response.setBody(file.whatever content)
            cli->_response.setHeader("Content-Length", getPathSize());
            cli->_response.setStatus(200);
        }

    }
    else if (cli->_request.getMethod() == "POST") {
    
    }
    else if (cli->_request.getMethod() == "DELETE") {
    
    }
    return (EXIT_SUCCESS);
}

bool RequestHandler::get_requested_ressource(Client* cli)
{
    struct stat fileInfo;
    std::string root_DIR; //get from conf, example "/var/www/html"

    //if requested ressource not found in root in the conf file cli->_response.setStatus(404); return(EXIT_FAILURE);
    std::string url = cli->_request.getURL();
    std::size_t query_pos = url.find("?");
    if (query_pos != std::string::npos)
        _path = url.substr(0, query_pos);
    else
        _path = url;

    std::string absolut_path = root_DIR + _path;
    if (stat(absolut_path.c_str(), &fileInfo) != 0)
    {
        cli->_response.setStatus(404);
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

const std::string RequestHandler::get_ressource_type(Client* cli)
{
    std::string pathType;
    struct stat fileInfo;
    stat(_path.c_str(), &fileInfo);
    if (S_ISDIR(fileInfo.st_mode))
       pathType = "DIR";
    else if (S_ISREG(fileInfo.st_mode))
        pathType = "FILE";
    return (pathType);
}

bool RequestHandler::is_dir_has_index_files(Client* cli)
{
    std::vector<std::string> indexFiles; //get it from cli->conf->indexFiles
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

bool RequestHandler::if_location_has_cgi(Client* cli)
{
    return (EXIT_SUCCESS);
}

const size_t RequestHandler::getPathSize()
{
    struct stat fileInfo;
    stat(_path.c_str(), &fileInfo);
    size_t pathSize = fileInfo.st_size;
    return (pathSize);
}


