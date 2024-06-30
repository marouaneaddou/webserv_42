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
{

}

void RequestHandler::req_uri_location(Client* cli)
{
    std::string url = cli->_request.getURL();
    std::size_t query_pos = url.find("?");
    if (query_pos != std::string::npos)
        _path = url.substr(0, query_pos);
    else
        _path = url;
    //Exact Match
    std::cout << "hona hona hona aban3ami\n";
    for (int i = 0; i < cli->getServer().locations.size(); i++)
    {
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
        std::cout << "LONGEST MATCH\n";
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
        if (cli->isOpen() == false && cli->getSizeFile() == -1)
            get_requested_ressource(cli);
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
                    return;
                }
            }
        }
        if (if_location_has_cgi(cli))
        {
            //run cgi
            //code depend on cgi
            // cli->_response.setStatus(200); ?
            fileData;
        int sizeFile;
        std::string dataFile;
        }
        else
        {
                // add tree variables to class Client 
                // (first , second) is fileData(in GET method create object with class ofstream to read data from file to my string(dataFile-> is second varible))
                // and third variable is sizeFile store size  to file and size of strean when send data from string to socket file 
            
            
            // first condition is check if file we need get is open or not and variable sizefile = -1 or 
            // note(sizefile initiaze with value -1 when create object class Client)
            // content -> CHECK ACCESS FILE AND OPEN FILE WE NEED TO SERVE

            //second condition check if file is open and varible sizeFile != 0 . 
            //if this condition is true read data from it's file to my string (dataFile)

            // third condition if varible sizeFile == 0 this meansthat we have read all data 
            // after that write data from string to socket (look WebServer.cpp )
            if (cli->isOpen() == false && cli->getSizeFile() == -1) 
            {
                if (access(_path.c_str(), R_OK) != 0)
                {
                    cli->_response.setStatus(403);
                    throw(403);
                }
                cli->openFile(_path.c_str());
            }
            else if (cli->isOpen() == true && cli->getSizeFile() != 0)
                cli->setData();
            if (cli->isOpen() == true && cli->getSizeFile() == 0)
            {

                cli->closeFile();
                cli->setSizeFile(cli->getData().size());
                std::cout << "start " <<std::endl << std::endl;
                // std::cout << cli->getData() << std::endl;
                std::cout << "end " <<std::endl << std::endl;
                // std::ofstream end("data.jpg");
                std::string StaticFile;// = getFileContent();
                // StaticFile += cli->getData();
                std::cout << "SGV" << std::endl;
                cli->_response.setHeader("Content-Type", getMimeType());
                cli->_response.setHeader("Content-Length", cli->getData().size()/*StaticFile.length()*/);
                cli->_response.setBody(cli->getData()/*StaticFile*/);
                cli->_response.generateResponseString();

            }
        }
    }
    else if (cli->_request.getMethod() == "POST")
    {
        get_requested_ressource(cli);
        // std::cout << "here status ok" << cli->_response.getStatus()  << " " << cli->getServer().locations[_blockIdx].getPath()<< std::endl;;
        if (get_ressource_type(cli) == "DIR")
        {
        }
        else 
        {
            // ifLocationSupportCgi(cli->getServer().locations[_blockIdx]);
            if (cli->getServer().locations[_blockIdx].getCgiSupport() != true)
            {
                cli->_response.setStatus(403);
                throw(403);
            }
            
            // std::ofstream outputFile("./test.jpg", std::ios::out);
            // std::cout << "{{{{{{{{{}}}}}}}}}\n";
            // if (outputFile.is_open())
            //     std::cout << "not open file\n";
            // cli->_request.getBody().find("\r\n");
            // std::string buffer = cli->_request.getBody().substr(cli->_request.getBody().find("\r\n\r\n") + 4);
            // outputFile.write(buffer.c_str(), buffer.length());
            // outputFile.close();
            // for (int i = 0; i < cli->_request.getSizeOfBodyPure(); i++)
            //     outputFile << cli->_request.getElementBodyPure(i);
            // outputFile.close();
            // std::cout << "hnaya kayan uploud" << std::endl;
            // std::cout << cli->_request.getPureBody[]
        }
    }
    else if (cli->_request.getMethod() == "DELETE") {
    
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
        std::string filePath = _path + cli->getServer()._indexFiles[i];
        if (stat(filePath.c_str(), &fileInfo) == 0 && S_ISREG(fileInfo.st_mode))
        {
            _path = _path + cli->getServer()._indexFiles[i];
            return (true);
        }
    }
    return (false);
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
    _path = cli->getServer().roots[0] + _path;
    std::cout << _path <<std::endl;
    if (stat(_path.c_str(), &fileInfo) != 0)
    {
        cli->_response.setStatus(404);
        cli->setSizeFile(0);
        std::cout << "here status" << cli->_response.getStatus() << std::endl;;
        throw(404);
    }
/*************** TEST *********/
    /*********** OPEN && READ ***********/
//    int fd = open(absolut_path.c_str(), O_RDONLY);
// if (fd == -1) {
//     perror("open");
//     cli->_response.setStatus(500); // Internal Server Error
//     return EXIT_FAILURE;
// }

// char str[fileInfo.st_size];
// int n = read(fd, str, fileInfo.st_size);
// if (n == -1) {
//     perror("read");
//     close(fd);
//     cli->_response.setStatus(500); // Internal Server Error
//     return EXIT_FAILURE;
// }
// close(fd);
// str[n] = '\0';
//     /*********** OPEN && READ ***********/      
//     std::string response= "HTTP/1.1 200 OK\r\n"
//                      "Date: Mon, 20 May 2024 12:34:56 GMT\r\n"
//                      "Server: Apache/2.4.41 (Ubuntu)\r\n";
//     response += "Accept";
//     response += ": ";
//     response += cli->_request.getHeader("Accept")->second;
//     response += "\r\n\r\n";
//     response += str;
//     /***************** WRITE IN NEW SOCKET****************/
//     size_t response_length = response.size();
//     ssize_t bytes_written = 1;
//         bytes_written = write(cli->socket, response.data(), response_length);

//     if (bytes_written == -1) 
//     {
//         perror("write");
//         cli->_response.setStatus(500); // Internal Server Error
//         return EXIT_FAILURE;
//     }
/***************** WRITE IN NEW SOCKET****************/
    // int nbyte = send(cli->socket, response.c_str(), strlen(response.c_str()), 0);
    // std::cout << "hello1" << fileInfo.st_size << std::endl;

/*************** TEST *********/

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
    MimeTypes[".css"] = "text/css";
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

    struct dirent* entry;
    std::string filePath;
    std::vector<std::string> curr;

        curr.push_back(".");
        curr.push_back("..");

    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_name == curr[0] || entry->d_name == curr[1])  
            continue;

        filePath = _path + "/" + entry->d_name;
            std::cout << entry->d_name << std::endl;;
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
    std::cout << cli->_response.getStatus() << std::endl;
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







