#include "RequestHandler.hpp"
#include <cstdlib>

RequestHandler::RequestHandler(){

}

bool RequestHandler::req_uri_location(Client* cli)
{
    std::string url = cli->_request.getURL();
    std::size_t query_pos = url.find("?");
    if (query_pos != std::string::npos)
        _path = url.substr(0, query_pos);
    else
        _path = url;

    //Exact Match
    for (int i = 0; i < cli->getServer().locations.size(); i++) 
    {
        if (cli->getServer().locations[i]->getPath()[0] == '=' && cli->getServer().locations[i]->getPath().substr(1) == _path)
        {
            _blockIdx = i;
            return (EXIT_SUCCESS);
        }
    }

    //Prefix Match
    std::string longestMatch;
    for (int i = 0; i < cli->getServer().locations.size(); i++)
    {
        if (cli->getServer().locations[i]->getPath()[0] != '=' && _path.find(cli->getServer().locations[i]->getPath()) == 0)
        {
            if (cli->getServer().locations[i]->getPath().length() > longestMatch.length())
            {
                longestMatch = cli->getServer().locations[i]->getPath();
                _blockIdx = i;
            }
        }
    }
    if (!(longestMatch.empty()))
        return(EXIT_SUCCESS);

    // default location Match
    if (cli->getServer().locations[0]->getPath() == "/")
    {
        _blockIdx = 0;
        return (EXIT_SUCCESS);
    }

    cli->_response.setStatus(404);
    return(EXIT_FAILURE);
}

bool RequestHandler::is_location_have_redirection(Client* cli)
{
    if (!(cli->getServer().locations[_blockIdx]->getReturn().empty()))
    {
        cli->_response.setHeader("Location", cli->getServer().locations[_blockIdx]->getReturn());
        cli->_response.setStatus(301);
        cli->_response.setHeader("Content-Length", 0);
        return(EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}

bool RequestHandler::is_method_allowed_in_location(Client* cli)
{
    for (int i = 0; i < cli->getServer().locations[_blockIdx]->getAcceptedMethod().size(); i++)
    {
        if (cli->getServer().locations[_blockIdx]->getAcceptedMethod()[i] == cli->_request.getMethod())
            return (EXIT_SUCCESS);
    }
    cli->_response.setStatus(405);
    return(EXIT_FAILURE);
}

bool RequestHandler::check_requested_method(Client* cli)
{

    if (cli->_request.getMethod() == "GET")
    {
        if (get_requested_ressource(cli) == EXIT_FAILURE) {return (EXIT_FAILURE);}
    
        if (get_ressource_type(cli) == "DIR")
        {
            if (_path[_path.length() - 1] != '/')
            {
                cli->_response.setHeader("Location", _path + '/');
                cli->_response.setHeader("Content-Length", 0);
                cli->_response.setStatus(301);
                return (EXIT_FAILURE);
            }
            if (is_dir_has_index_files(cli) == EXIT_FAILURE)
            {
                //check autoindex/directorylisting : ON/OFF

                //OFF :
                if (cli->getServer().locations[_blockIdx]->directory_listing == false)
                {
                    cli->_response.setStatus(403);
                    return(EXIT_FAILURE);
                }
                //ON :
                else
                {
                    cli->_response.setHeader("Content-Type", "text/html");
                    // cli->_response.setBody(HTML page with the content in the directory)
                    // cli->_response.setHeader("Content-Length", "size of resp body in bytes")
                    return(EXIT_SUCCESS);
                }
            }
        }
        if (if_location_has_cgi(cli))
        {
            //run cgi
            //code depend on cgi
            // cli->_response.setStatus(200); ?
        }
        else
        {
            //return requested file cli->_response.setBody(file.whatever content)
            cli->_response.setHeader("Content-Length", getPathSize());
            cli->_response.setStatus(200);
        }

    }
    else if (cli->_request.getMethod() == "POST")
    {
        
    }
    else if (cli->_request.getMethod() == "DELETE") {
    
    }
    return (EXIT_SUCCESS);
}

bool RequestHandler::get_requested_ressource(Client* cli)
{
    
    struct stat fileInfo;

    std::string url = cli->_request.getURL();
    std::size_t query_pos = url.find("?");
    if (query_pos != std::string::npos)
        _path = url.substr(0, query_pos);
    else
        _path = url;
    std::string absolut_path = cli->getServer().roots[0] + _path;
        std::cout << "ABS "<< absolut_path << std::endl;
    if (stat(absolut_path.c_str(), &fileInfo) != 0)
    {
        cli->_response.setStatus(404);
        return (EXIT_FAILURE);
    }
/*************** TEST *************/
    // int fd = open(absolut_path.c_str(), O_RDONLY | O_CREAT);
    // char str[540];
    // int n = read(fd, str, 540);
    // str[n] = '\0';
    // response= "HTTP/1.1 200 OK\r\n"
    //                  "Date: Mon, 20 May 2024 12:34:56 GMT\r\n"
    //                  "Server: Apache/2.4.41 (Ubuntu)\r\n";
    // response += "Accept";
    // response += ": ";
    // response += cli->_request.getHeader("Accept")->second;
    // response += "\r\n\r\n";
    // response += str;
    // int nbyte = send(cli->socket, response.c_str(), strlen(response.c_str()), 0);
    // std::cout << "response: " << response.c_str() << std::endl;
/*************** TEST ************/
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
    struct stat fileInfo;

    for (int i = 0; i < cli->getServer()._indexFiles.size(); i++)
    {
        std::string filePath = _path + cli->getServer()._indexFiles[i];
        if (stat(filePath.c_str(), &fileInfo) == 0 && S_ISREG(fileInfo.st_mode))
        {
            _path = _path + cli->getServer()._indexFiles[i];
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


void RequestHandler::setStatusMessage(Client* cli)
{
    switch(cli->_response.getStatus()) 
    {
        case 501:
            cli->_response.setStatusMsg("Not Implemented");
        break;
        case 400:
            cli->_response.setStatusMsg("Bad Request");
        break;
        case 414:
            cli->_response.setStatusMsg("Request-URI Too Large");
        case 413:
            cli->_response.setStatusMsg("Request Entity Too Large");
        break;
        case 404:
            cli->_response.setStatusMsg("Not Found");
        break;
        case 301:
            cli->_response.setStatusMsg("Moved Permanently");
        break;
        case 405:
            cli->_response.setStatusMsg("Method Not Allowed");
        break;
        case 403:
            cli->_response.setStatusMsg("Forbidden");
        break;
        case 409:
            cli->_response.setStatusMsg("Conflict");
        break;
        case 204:
            cli->_response.setStatusMsg("No Content");
        break;
        case 500:
            cli->_response.setStatusMsg("Internal Server Error");
        break;
        case 201:
            cli->_response.setStatusMsg("Created");
        default:
            cli->_response.setStatusMsg("OK");
    }
}

// bool RequestHandler::if_location_support_upload(Client* cli)
// {

// }


