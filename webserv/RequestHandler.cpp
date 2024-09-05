#include "RequestHandler.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <string>
// #include <sys/dirent.h>
#include <sys/unistd.h>
#include <unistd.h>

RequestHandler::RequestHandler()
{}

void RequestHandler::req_uri_location(Client* cli)
{
    std::string url = cli->_request.getURL();
    std::cout << "url " << url << std::endl;
    std::size_t query_pos = url.find("?");
    if (query_pos != std::string::npos)
        _path = url.substr(0, query_pos);
    else
        _path = url;
    //Exact Match
    for (int i = 0; i < cli->getServer().locations.size(); i++)
    {
        std::cout << cli->getServer().locations[i].getPath() << std::endl;
        if (cli->getServer().locations[i].getPath()[0] == '=' && cli->getServer().locations[i].getPath().substr(2) == _path)
        {
            _blockIdx = i;
            return;
        }
    }
    //Prefix Match + default if no longer prefix found
    std::string longestMatch;
    for (int i = 0; i < cli->getServer().locations.size(); i++)
    {
        if (cli->getServer().locations[i].getPath()[0] != '=' && _path.find(cli->getServer().locations[i].getPath()) == 0)
        {
            if (cli->getServer().locations[i].getPath().length() > longestMatch.length())
            {
                longestMatch = cli->getServer().locations[i].getPath();
                _blockIdx = i;
            }
        }
    }
    std::cout << "path -->" << "'"<<longestMatch << "'"<<std::endl;
    if (!(longestMatch.empty()))
    {
        //std::cout << "LONGEST MATCH\n";
        return;
    }
    // cli->_response.setStatus(404); // default location is always there, we dont need this
    // throw(404);
}

void RequestHandler::is_location_have_redirection(Client* cli)
{
    if (!(cli->getServer().locations[_blockIdx].getReturn().empty()))
    {
        // std::cout << cli->getServer().locations[_blockIdx].getReturn() << std::endl;
        cli->_response.setHeader("Location", cli->getServer().locations[_blockIdx].getReturn());
        cli->_response.setStatus(301);
        cli->_response.setHeader("Content-Length", 0);
        throw(301);
    }
    return;
}

void RequestHandler::is_method_allowed_in_location(Client* cli)
{
    for (int i = 0; i < cli->getServer().locations[_blockIdx].getAcceptedMethod().size(); i++)
    {
        // std::cout << "HERE --------------???????????\n";
        if (cli->getServer().locations[_blockIdx].getAcceptedMethod()[i] == cli->_request.getMethod())
            return;
    }
    cli->_response.setStatus(405);
    throw(405);
}

void RequestHandler::check_requested_method(Client* cli)
{

    if (cli->_request.getMethod() == "GET")
    {
        if(cli->getOnetime() == false)
        {
            cli->setOnetime();
            get_requested_ressource(cli);
            // std::cout << " request ==> " << get_requested_ressource(cli) << std::endl;
            if (get_ressource_type(cli) == "DIR")
            {
                if (_path[_path.length() - 1] != '/')
                {
                    cli->_response.setHeader("Location", _path + '/');
                    cli->_response.setHeader("Content-Length", 0);
                    cli->_response.setStatus(301);
                    throw(301);
                }
                if (is_dir_has_index_files(cli) == false)
                {
                    //check autoindex/directorylisting : ON/OFF

                    //OFF :
                    if (cli->getServer().locations[_blockIdx].directory_listing == false)
                    {
                        cli->_response.setStatus(403);
                        throw(403);
                    }
                    //ON :
                    else
                    {
                        std::string htmlFile = getDirListing();
                        cli->_response.setHeader("Content-Type", "text/html");
                        cli->_response.setHeader("Content-Length", htmlFile.length());
                        cli->_response.setBody(htmlFile);
                        cli->setTypeData(WRITEDATA);
                        cli->_response.generateHeaderResponse();
                        return;
                    }
                }
            }
            else 
            {
                if (access(abs_path.c_str(), R_OK) != 0)
                {
                    cli->_response.setStatus(403);
                    throw(403);
                }
                cli->openFile(abs_path.c_str());
                cli->setTypeData(READDATA);
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
            if (cli->getTypeData() == READDATA)
            {
                cli->setData();
                //std::cout << "sizefile " <<cli->getSizeFile() << std::endl;
                //std::cout << "xxxxxxxxxxxxxxxxxx " <<_path << std::endl;
                if (cli->getreadWriteSize() == cli->getSizeFile())
                {
                    cli->setTypeData(WRITEDATA);
                    cli->setreadWriteSize(0);
                    cli->_response.setHeader("Content-Type", getMimeType());
                    // cli->_response.setHeader("Content-Length", cli->_response.getBody().size()/*StaticFile.length()*/);
                    cli->_response.setHeader("Content-Length", cli->getData().size()/*StaticFile.length()*/);
                    cli->closeFile();
                    cli->_response.setBody(cli->getData()/*StaticFile*/);
                    cli->_response.generateHeaderResponse();
                }
            }
        }
    }
    else if (cli->_request.getMethod() == "POST")
    {
        // check location have or support uploud 
        // cli->_request.getHeader();
        if (cli->getOnetime() == false) {
            cli->setOnetime();
            get_requested_ressource(cli);

            if (get_ressource_type(cli) == "DIR") {
                if (_path[_path.length() - 1] != '/') {
                    cli->_response.setHeader("Location", _path + '/');
                    cli->_response.setHeader("Content-Length", 0);
                    cli->_response.setStatus(301);
                    throw(301);
                }

                std::cout << "have index file " << is_dir_has_index_files(cli)  << std::endl;
                if (is_dir_has_index_files(cli) == true) {
                    // std::cout << ;
                    // add abs URL and index file 
                    
            std::cout << "{{{{{{{{{{{{{{{}}}}}}}}}}}}}}}"<< std::endl;
                    // abs_path += cli->getServer()._indexFiles[0];
                    std::cout << "abs path" << abs_path << std::endl;
                    if (access(abs_path.c_str(), R_OK) != 0) {
                        cli->_response.setStatus(403);
                        throw(403);
                    }
                    if (cli->getServer().locations[_blockIdx].getCgiSupport() == 1)  {
                    //excuteFile();
                    }
                    else {
                        cli->_response.setStatus(403);
                        throw(403);
                    }
                }
                else {
                    cli->_response.setStatus(403);
                    throw(403);
                } 
            }
            // else {
            //     if (access(abs_path.c_str(), R_OK) != 0) {
            //         cli->_response.setStatus(403);
            //         throw(403);
            //     }
            //     std::cout << "permission is ok" << std::endl;
            // }
        }
        cli->setTypeData(CLOSESOCKET);




        

        //     get_requested_ressource(cli);
        // // std::cout << "here status ok" << cli->_response.getStatus()  << " " << cli->getServer().locations[_blockIdx].getPath()<< std::endl;;
        // if (get_ressource_type(cli) == "DIR")
        // {
        // }
        // else 
        // {
        //     // ifLocationSupportCgi(cli->getServer().locations[_blockIdx]);
        //     if (cli->getServer().locations[_blockIdx].getCgiSupport() != true)
        //     {
        //         cli->_response.setStatus(403);
        //         throw(403);
        //     }
            
        //     // std::ofstream outputFile("./test.jpg", std::ios::out);
        //     // std::cout << "{{{{{{{{{}}}}}}}}}\n";
        //     // if (outputFile.is_open())
        //     //     std::cout << "not open file\n";
        //     // cli->_request.getBody().find("\r\n");
        //     // std::string buffer = cli->_request.getBody().substr(cli->_request.getBody().find("\r\n\r\n") + 4);
        //     // outputFile.write(buffer.c_str(), buffer.length());
        //     // outputFile.close();
        //     // for (int i = 0; i < cli->_request.getSizeOfBodyPure(); i++)
        //     //     outputFile << cli->_request.getElementBodyPure(i);
        //     // outputFile.close();
        //     // std::cout << "hnaya kayan uploud" << std::endl;
        //     // std::cout << cli->_request.getPureBody[]
        // }
    }
    else if (cli->_request.getMethod() == "DELETE") {
        if(cli->getOnetime() == false)
        {
            cli->setOnetime();
            get_requested_ressource(cli);
            if (get_ressource_type(cli) == "DIR")
            {
                if (_path[_path.length() - 1] != '/')
                {
                    cli->_response.setStatus(409);
                    throw(409);
                }
                if (if_location_has_cgi(cli))
                {
                    if (is_dir_has_index_files(cli) == false)
                    {
                        
                    }
                }
                else
                    ;
                    //delete all
                
            }

        }

    
    }
    return;
}

// bool RequestHandler::ifLocationSupportCgi(Location &location) const
// {
//     return 
// }


bool RequestHandler::is_dir_has_index_files(Client* cli)
{
    struct stat fileInfo;

    for (int i = 0; i < cli->getServer()._indexFiles.size(); i++)
    {
        std::string filePath = abs_path + cli->getServer()._indexFiles[i];
        if (stat(filePath.c_str(), &fileInfo) == 0 && S_ISREG(fileInfo.st_mode))
        {
            abs_path = abs_path + cli->getServer()._indexFiles[i];
            std::cout << abs_path << " " << cli->getServer()._indexFiles[i] << std::endl;
            return true;
        }
    }
    std::cout << "pppppppppppppppppppppp" << std::endl;
    return false;
}

bool RequestHandler::if_location_has_cgi(Client* cli)
{
    return (EXIT_SUCCESS);
}


////////////////getters/////////////////
void RequestHandler::get_requested_ressource(Client* cli)
{
    struct stat fileInfo;

    std::string url = cli->_request.getURL();
    std::size_t query_pos = url.find("?");
    
    if (query_pos != std::string::npos)
        _path = url.substr(0, query_pos);
    else
        _path = url;
    abs_path = cli->getServer().roots[0] + _path;
    std::cout << "absulate path ++> " << abs_path << std::endl;
    if (stat(abs_path.c_str(), &fileInfo) != 0) {
        std::cout << "ERROR" << std::endl;
        cli->_response.setStatus(404);
        //cli->setSizeFile(0);
        throw(404);
    }
}

const std::string RequestHandler::get_ressource_type(Client* cli)
{
    std::string pathType;
    struct stat fileInfo;

    stat(abs_path.c_str(), &fileInfo);
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
    MimeTypes[".css"] = "text/css";
    MimeTypes[".mp4"] = "video/mp4";
    //std::cout << "PATH" << _path << std::endl;
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
    htmlContent += "    <h1>Directory Listing for " + abs_path + "</h1>\n";
    htmlContent += "    <table>\n";
    htmlContent += "        <tr><th>Name</th><th>Size</th></tr>\n";

    DIR* dir = opendir(abs_path.c_str());

    struct dirent* entry;
    std::string filePath;
    std::vector<std::string> curr;

        curr.push_back(".");
        curr.push_back("..");

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name == curr[0] || entry->d_name == curr[1])  
            continue;

        filePath = abs_path + "/" + entry->d_name;
            // std::cout << entry->d_name << std::endl;;
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
        break;
        case 200:
            cli->_response.setStatusMsg("OK");
        break;
    }
}







