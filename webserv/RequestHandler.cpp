#include "RequestHandler.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <string>
#include <sys/dirent.h>
#include <sys/unistd.h>
#include <unistd.h>

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
                    std::string htmlFile = getDirListing();
                    cli->_response.setHeader("Content-Type", "text/html");
                    cli->_response.setHeader("Content-Length", htmlFile.length());
                    cli->_response.setBody(htmlFile);
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
            if (access(_path.c_str(), R_OK) != 0)
            {
                cli->_response.setStatus(403);
                return(EXIT_FAILURE);
            }
            std::string StaticFile = getFileContent();
            cli->_response.setHeader("Content-Type", getMimeType());
            cli->_response.setHeader("Content-Length", StaticFile.length());
            cli->_response.setBody(StaticFile);

        }
        return (EXIT_SUCCESS);

    }
    else if (cli->_request.getMethod() == "POST")
    {
        
    }
    else if (cli->_request.getMethod() == "DELETE") {
    
    }
    return (EXIT_SUCCESS);
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


////////////////getters/////////////////
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

const size_t RequestHandler::getPathSize()
{
    struct stat fileInfo;
    stat(_path.c_str(), &fileInfo);
    size_t pathSize = fileInfo.st_size;
    return (pathSize);
}
const std::string RequestHandler::getFileContent()
{
    std::string body;
    int fd = open(_path.c_str(), O_RDONLY);
    if (fd < 0)
    {
        perror("Error");
        exit(1);
    }
    int bytes = getPathSize();
    char str[bytes];
    int n = read(fd, str, bytes);
    if (n < 0)
    {
        perror("Error");
        exit(1);
    }
    str[n] = '\0';
    body += str;

    return (body);
}
const std::string RequestHandler::getMimeType()
{
    std::map<std::string, std::string> MimeTypes;
    MimeTypes[".html"] = "text/html";
    MimeTypes[".jpg"] = "image/jpeg";
    MimeTypes[".png"] = "image/png";
    MimeTypes[".gif"] = "image/gif";
    MimeTypes[".txt"] = "text/plain";

    std::string::size_type idx = _path.rfind('.');
    if (idx != std::string::npos) 
    {
        std::string extension = _path.substr(idx);
        std::map<std::string, std::string>::iterator it = MimeTypes.find(extension);
        if (it != MimeTypes.end()) 
            return it->second;
    }
    return "Unknown";
}
const std::string RequestHandler::getDirListing()
{
     std::string htmlContent;

    htmlContent += "<!DOCTYPE html>\n";
    htmlContent += "<html lang=\"en\">\n";
    htmlContent += "<head>\n";
    htmlContent += "    <meta charset=\"UTF-8\">\n";
    htmlContent += "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
    htmlContent += "    <title>Directory Listing</title>\n";
    htmlContent += "    <style>\n";
    htmlContent += "        table { width: 100%; border-collapse: collapse; }\n";
    htmlContent += "        th, td { padding: 8px 12px; border: 1px solid #ccc; }\n";
    htmlContent += "        th { background-color: #f4f4f4; }\n";
    htmlContent += "    </style>\n";
    htmlContent += "</head>\n";
    htmlContent += "<body>\n";
    htmlContent += "    <h1>Directory Listing for " + _path + "</h1>\n";
    htmlContent += "    <table>\n";
    htmlContent += "        <tr><th>Name</th><th>Size</th></tr>\n";

    DIR* dir = opendir(_path.c_str());
    if (dir == NULL)
    {
        std::cerr << "Invalid directory path." << std::endl;
        exit(1);
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL)
    {
        std::string filePath = _path + "/" + entry->d_name;
        struct stat fileStat;

        stat(filePath.c_str(), &fileStat);

        htmlContent += "        <tr><td>";
        if (S_ISDIR(fileStat.st_mode)) 
        {
            std::string directoryName = entry->d_name;
            if (!directoryName.empty() && directoryName.back() != '/')
                directoryName += "/";

            htmlContent += "<a href=\"" + directoryName + "\">" + entry->d_name + "/</a>";
        }
        else
            htmlContent += "<a href=\"" + std::string(entry->d_name) + "\">" + entry->d_name + "</a>";

        htmlContent += "</td>";

        if (S_ISREG(fileStat.st_mode))
            htmlContent += "<td>" + std::to_string(fileStat.st_size) + " bytes</td>";
        else
            htmlContent += "<td> - </td>";

        htmlContent += "</tr>\n";
    }
    closedir(dir);
    
    htmlContent += "    </table>\n";
    htmlContent += "</body>\n";
    htmlContent += "</html>\n";

    return (htmlContent);
}
//////////////////////////////////////////////////


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







