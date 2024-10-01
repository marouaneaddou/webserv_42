#include "../includes/RequestHandler.hpp"


RequestHandler::RequestHandler()
{
    _path = "";
    abs_path = "";
    _blockIdx = -1;
}

// std::string generateHTML_file(std::string print, bool type) {
//     std::string htmlfile = "<!DOCTYPE html>\n";
//     htmlfile += "<html lang=\"en\">\n";
//     htmlfile += "<head>\n";
//     htmlfile += "    <meta charset=\"UTF-8\">\n";
//     htmlfile += "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n";
//     htmlfile += "    <title>Document</title>\n";
//     htmlfile += "</head>\n";
//     htmlfile += "<body>\n";
//     htmlfile += "    <div>\n";
//     if (type == 0) htmlfile += "        <p style=\"color: brown; font-size: 35px;\">"+ print + "</p>\n";
//     else htmlfile += "        <p style=\"color: green; font-size: 35px;\">"+ print + "</p>\n";
//     htmlfile += "    </div>\n";
//     htmlfile += "</body>\n";
//     htmlfile += "</html>\n";
//     return htmlfile;
// }

std::string generateHTML_file(std::string print, bool type, int status) {
    (void)type;
    std::string html = "<!DOCTYPE html>\n"
                   "<html lang=\"en\">\n"
                   "<head>\n"
                   "    <meta charset=\"UTF-8\">\n"
                   "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                   "    <title>Document</title>\n"
                   "    <style>\n"
                   "        body {\n"
                   "            background-color: black;\n"
                   "            display: flex;\n"
                   "            justify-content: center;\n"
                   "            align-items: center;\n"
                   "            height: 100vh;\n"
                   "            margin: 0;\n"
                   "        }\n"
                   "        .container {\n"
                   "            width: 600px;\n"
                   "            height: 400px;\n"
                   "            border: 1px solid black;\n"
                   "            border-radius: 25px;\n"
                   "            background-color: white;\n"
                   "        }\n"
                   "        .container p {\n"
                   "            color: brown;\n"
                   "            text-align: center;\n"
                   "        }\n"
                   "    </style>\n"
                   "</head>\n"
                   "<body>\n"
                   "    <div class=\"container\">\n"
                   "        <p style=\"font-size: 80px;\">Oops !</p>\n"
                   "        <p style=\"font-size: 30px;\"><span style=\"font-size: 50px;\">"+ std::to_string(status) + "</span> : " + print + "</p>\n"
                   "    </div>\n"
                   "</body>\n"
                   "</html>";
    return html;

}

// std::string readFile(std::string path, std::string exactPath) {

// }

void RequestHandler::req_uri_location(Client* cli)
{
    std::string url = cli->_request.getURL();
    std::size_t query_pos = url.find("?");
    if (query_pos != std::string::npos)
        _path = url.substr(0, query_pos);
    else
        _path = url;
    //Exact Match
    for (unsigned int i = 0; i < cli->getServer().get_locations().size(); i++)
    {
        if (cli->getServer().get_locations()[i].getPath()[0] == '=' && cli->getServer().get_locations()[i].getPath().substr(2) == _path)
        {
            _blockIdx = i;
            return;
        }
    }
    //Prefix Match + default if no longer prefix found
    std::string longestMatch;
    for (unsigned int i = 0; i < cli->getServer().get_locations().size(); i++)
    {
        if (cli->getServer().get_locations()[i].getPath()[0] != '=' && _path.find(cli->getServer().get_locations()[i].getPath()) == 0)
        {
            if (cli->getServer().get_locations()[i].getPath().length() > longestMatch.length())
            {
                longestMatch = cli->getServer().get_locations()[i].getPath();
                _blockIdx = i;
            }
        }
    }
    if (!(longestMatch.empty()))
    {
        return;
    }
    
}

void RequestHandler::is_location_have_redirection(Client* cli)
{
    if (!(cli->getServer().get_locations()[_blockIdx].getReturn().empty()))
    {
        std::cout << "RETURN FILE: " << cli->getServer().get_locations()[_blockIdx].getReturn() << std::endl;
        cli->_response.setHeader("Location", cli->getServer().get_locations()[_blockIdx].getReturn());
        cli->_response.setStatus(301);
        cli->_response.setHeader("Content-Length", 0);
        throw(301);
    }
    return;
}

void RequestHandler::is_method_allowed_in_location(Client* cli)
{
    for (unsigned int i = 0; i < cli->getServer().get_locations()[_blockIdx].getMethods().size(); i++)
    {
        if (cli->getServer().get_locations()[_blockIdx].getMethods()[i] == cli->_request.getMethod())
            return;
    }
    cli->_response.setStatus(405);
    throw(405);
}



char **custom_cgi_envp()
{
    // std::map<std::string, std::string>::iterator it = request_headers.find("Content-Length");
    // if (it != request_headers.end())
    //     envp_as_vec.push_back("CONTENT_LENGTH=" + it->second);
    // it = request_headers.find("Content-Type");
    // if (it != request_headers.end())
    // envp_as_vec.push_back("CONTENT_TYPE=" + it->second);
    // envp_as_vec.push_back("SCRIPT_FILENAME=" + requested_resource_path);
    // envp_as_vec.push_back("SCRIPT_NAME=" +  request_url_as_vector.back());
    // envp_as_vec.push_back("QUERY_STRING=");
    // envp_as_vec.push_back("REQUEST_METHOD=" + request_method);
    // envp_as_vec.push_back("REDIRECT_STATUS=200");
    // envp_as_vec.push_back("SERVER_NAME=webserv");
    // envp_as_vec.push_back("SERVER_PROTOCOL=HTTP/1.1");


    std::vector<std::string> envp_as_vec;
    envp_as_vec.push_back("CONTENT_TYPE=text/html");
    envp_as_vec.push_back("CONTENT_LENGTH=0");
    envp_as_vec.push_back("SCRIPT_FILENAME=/path/to/your-cgi-script.py");
    envp_as_vec.push_back("SCRIPT_NAME=/cgi-bin/your-cgi-script.py");
    envp_as_vec.push_back("QUERY_STRING=");
    envp_as_vec.push_back("REQUEST_METHOD=GET");
    envp_as_vec.push_back("REDIRECT_STATUS=200");
    envp_as_vec.push_back("SERVER_NAME=webserv");
    envp_as_vec.push_back("SERVER_PROTOCOL=HTTP/1.1");

    char **envp;
    int envp_size = envp_as_vec.size();
    envp = (char **)malloc((envp_size + 1) * sizeof(char *));

    for (int i = 0; i < envp_size; i++)
    {
        envp[i] = (char *)malloc(envp_as_vec[i].size() + 1); 
        strcpy(envp[i], envp_as_vec[i].c_str());              
    }

    envp[envp_size] = NULL;
    return (envp);
}



std::string cgi_exec(std::string _path, int flag_python)
{
    std::string cgi_path = _path;
    char **envp = custom_cgi_envp();
    std::string cgi_response;
    char **av;
    av = (char **)malloc(3 * sizeof(char *));

    av[0] = (char *)malloc(strlen("/usr/local/bin/python3") + 1);
    strcpy(av[0], "/usr/local/bin/python3");
    av[1] = (char *)malloc(cgi_path.size() + 1);
    strcpy(av[1], cgi_path.c_str());
    av[2] = NULL;

    int fd[2];
    pipe(fd);
    if (flag_python == 1){
        pid_t pid = fork();
        if (pid == 0){
            close(fd[0]);
            dup2(fd[1], 1);
            execve(av[0], av, envp);
            exit(0);
        }
        else{
            close(fd[1]);
            char buffer[1024];
            int len = read(fd[0], buffer, 1024);
            buffer[len] = '\0';
            cgi_response = buffer;
            waitpid(pid, NULL, 0);

        }
    }
    else
    {
        std::ifstream file(cgi_path);
        std::string str;
        std::string cgi_response;
        while (std::getline(file, str))
        {
            cgi_response += str;
        }
    }
    
    return (cgi_response);

}


void RequestHandler::check_requested_method(Client* cli)
{

    if (cli->_request.getMethod() == "GET")
    {
        if(cli->getOnetime() == false)
        {
            cli->setOnetime();
            get_requested_ressource(cli);
            std::string typeRessource = get_ressource_type(abs_path);
            if (typeRessource == "DIR")
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
                // std::cout <<"path =>> " << _path[_path.length() - 1] << std::endl;
                    //check autoindex/directorylisting : ON/OFF

                    //OFF :
                    if (cli->getServer().get_locations()[_blockIdx].getDirectoryListing() == false)
                    {
                        cli->_response.setStatus(403);
                        cli->_response.setHeader("Content-Length", 0);
                        throw(403);
                    }
                    //ON :
                    else
                    {
                        std::string htmlFile = getDirListing();
                        cli->_response.setHeader("Content-Type", "text/html");
                        cli->_response.setHeader("Content-Length", htmlFile.length());
                        cli->_response.setHeader("Accept", "*/*");
                        cli->_response.setBody(htmlFile);
                        cli->setTypeData(WRITEDATA);
                        cli->_response.generateHeaderResponse();
                        return;
                    }
                }
            }
            if (access(abs_path.c_str(), R_OK) != 0)
            {
                cli->_response.setStatus(403);
                throw(403);
            }
            cli->openFile(abs_path.c_str());
            cli->setTypeData(READDATA);
        }
        if (cli->getServer().get_locations()[_blockIdx].getCgiSupport() == 1 && cli->checkExtensionFile(abs_path))  {
            std::string cgi_response = cgi_exec(abs_path, 1);
            cli->_response.setStatus(200);                         
            std::string htmlfile = generateHTML_file(cgi_response, 1, 200);
            cli->_response.setHeader("Content-Type", "text/html");
            cli->_response.setHeader("Content-Length", htmlfile.length());
            cli->_response.setBody(htmlfile);
            cli->_response.generateHeaderResponse();
            cli->setTypeData(WRITEDATA);
            throw(200);
        }
        else
        {
            if (cli->getTypeData() == READDATA)
            {
                cli->setData();
                if (cli->getreadWriteSize() == cli->getSizeFile())
                {
                    cli->setTypeData(WRITEDATA);
                    cli->setreadWriteSize(0);
                    cli->_response.setHeader("Content-Type", getMimeType());
                    cli->_response.setHeader("Content-Length", cli->getData().size());
                    cli->closeFile();
                    cli->_response.setBody(cli->getData());
                    cli->_response.generateHeaderResponse();
                }
            }
        }
    }
    else if (cli->_request.getMethod() == "POST")
    {
        
            get_requested_ressource(cli);
            cli->setOnetime();
            if (get_ressource_type(abs_path) == "DIR") {
                if (_path[_path.length() - 1] != '/') {
                    if (access(abs_path.c_str(), R_OK) != 0) {
                        std::string htmlfile = generateHTML_file("ERROR: forbiden access", 0, 403);
                        cli->_response.setHeader("Content-Type", "text/html");
                        cli->_response.setHeader("Content-Length", htmlfile.length());
                        cli->_response.setBody(htmlfile);
                        cli->_response.generateHeaderResponse();
                        cli->_response.setStatus(403);
                        cli->setTypeData(WRITEDATA);
                        throw(403);
                    }
                    cli->_response.setHeader("Location", _path + '/');
                    cli->_response.setStatus(307);
                    throw(301);
                }
            
                if (is_dir_has_index_files(cli) == true) {
                    if (access(abs_path.c_str(), R_OK) != 0) {
                        
                        std::string htmlfile = generateHTML_file("ERROR: forbiden access", 0, 403);
                        cli->_response.setHeader("Content-Type", "text/html");
                        cli->_response.setHeader("Content-Length", htmlfile.length());
                        cli->_response.setBody(htmlfile);
                        cli->_response.generateHeaderResponse();
                        cli->_response.setStatus(403);
                        cli->setTypeData(WRITEDATA);
                        throw(403);
                    }
                    if (cli->getServer().get_locations()[_blockIdx].getCgiSupport() == 1)  {
                                        /*****************************************/


                                                    /***************CGI CGI CGI !!!!!!!!!!!!!*****************/


                                        /*****************************************/

                                        // check return cgi !!!!!!!!!!
                                        /*****this just test ====>***/ cli->_response.setStatus(200);

                                        /****************************************/
                                        std::string htmlfile = generateHTML_file("The file upload was successful.", 1, 200);

                                        cli->_response.setHeader("Content-Type", "text/html");
                                        cli->_response.setHeader("Content-Length", htmlfile.length());
                                        cli->_response.setBody(htmlfile);
                                        cli->_response.generateHeaderResponse();
                                        cli->setTypeData(WRITEDATA);
                        /****************************************/
                        // throw(200);
                    }
                    else {
                        // std::string htmlfile;
                        // if (cli->getServer().get_error_pages().find("403") != cli->getServer().get_error_pages().end()) {
                        //     htmlfile
                        // }
                        std::string htmlfile = generateHTML_file("ERROR: NOT Support CGI", 0, 403);
                        cli->_response.setStatus(403);
                        cli->_response.setBody(htmlfile);
                        cli->_response.setHeader("Content-Type", "text/html");
                        cli->_response.setHeader("Content-Length", htmlfile.length());
                        cli->_response.generateHeaderResponse();
                        cli->setTypeData(WRITEDATA);
                        throw(403);
                    }
                }
                else {
                        
                    std::string htmlfile = generateHTML_file("ERROR: NOT index file", 0, 403);
                    // cli->_response.setStatus(403);
                    cli->_response.setBody(htmlfile);
                    cli->_response.setHeader("Content-Type", "text/html");
                    cli->_response.setHeader("Content-Length", htmlfile.length());
                    cli->_response.setStatus(403);
                    cli->_response.generateHeaderResponse();
                    cli->setTypeData(WRITEDATA);
                    throw(403);
                } 
            }
            else {
                 if (access(abs_path.c_str(), R_OK) != 0) {
                        std::string htmlfile = generateHTML_file("ERROR: forbiden access", 0, 403);
                        cli->_response.setHeader("Content-Type", "text/html");
                        cli->_response.setHeader("Content-Length", htmlfile.length());
                        cli->_response.setBody(htmlfile);
                        cli->_response.generateHeaderResponse();
                        cli->_response.setStatus(403);
                        cli->setTypeData(WRITEDATA);
                        throw(403);
                    }
                    // check location have or support uploud 
                    if (cli->getServer().get_locations()[_blockIdx].getCgiSupport() == 1)  {
                                        /*****************************************/


                                                    /***************CGI CGI CGI !!!!!!!!!!!!!*****************/


                                        /*****************************************/

                                        // check return cgi !!!!!!!!!!
                                        /*****this just test ====>***/ cli->_response.setStatus(200);

                                        /****************************************/
                                         std::string htmlfile = generateHTML_file("The file upload was successful.", 1, 200);

                                        cli->_response.setHeader("Content-Type", "text/html");
                                        cli->_response.setHeader("Content-Length", htmlfile.length());
                                        cli->_response.setBody(htmlfile);
                                        cli->_response.generateHeaderResponse();
                                        cli->setTypeData(WRITEDATA);
                        /****************************************/
                        // throw(200);
                    }
                    else {
                        std::string htmlfile = generateHTML_file("ERROR: NOT seport CGI", 0, 403);
                        cli->_response.setStatus(403);
                        cli->_response.setBody(htmlfile);
                        cli->_response.setHeader("Content-Type", "text/html");
                        cli->_response.setHeader("Content-Length", htmlfile.length());
                        cli->_response.generateHeaderResponse();
                        cli->setTypeData(WRITEDATA);
                        throw(403);
                    }
            }

    }
    else if (cli->_request.getMethod() == "DELETE") 
    {
        get_requested_ressource(cli);
        handleDeleteRequest(cli, abs_path);
        cli->_response.setStatus(204);
        throw(204);
    }
    return;
}

//////////////////////
void RequestHandler::deleteDirectoryRecursively(Client* cli, const char* dirPath)
{
     DIR *dir = opendir(dirPath);
    if (dir == NULL)
    {
        
        cli->_response.setStatus(403);
        throw(403);
    }
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        std::string fullPath = std::string(dirPath) + "/" + entry->d_name;
        if (entry->d_type == DT_DIR)
            deleteDirectoryRecursively(cli, fullPath.c_str());
        else
            handleDeleteRequest(cli, fullPath);
    }
    closedir(dir);
    rmdir(dirPath);
}
void RequestHandler::handleDeleteRequest(Client* cli, std::string abs_path)
{
    if (get_ressource_type(abs_path) == "DIR")
    {
        if (_path[_path.length() - 1] != '/')
        {
            cli->_response.setStatus(409);
            throw(409);
        }
        if(cli->getServer().get_locations()[_blockIdx].getCgiSupport() == 1)
        {
            if (is_dir_has_index_files(cli) == true)
                std::string cgi_response = cgi_exec(abs_path, 1);
            else
            {
                
                cli->_response.setStatus(403);
                throw(403);
            }
        }
        else
            deleteDirectoryRecursively(cli, abs_path.c_str());

    }
    else
    {
        if(cli->getServer().get_locations()[_blockIdx].getCgiSupport() == 1)
            std::string cgi_response = cgi_exec(abs_path, 1);
        else
            std::remove(abs_path.c_str());  
    }
}



bool RequestHandler::is_dir_has_index_files(Client* cli)
{
    struct stat fileInfo;
    std::cout << cli->getServer().get_locations()[_blockIdx].getIndexFiles().size() << std::endl;
    for (unsigned int i = 0; i < cli->getServer().get_locations()[_blockIdx].getIndexFiles().size(); i++)
    {
        std::string filePath = abs_path + cli->getServer().get_locations()[_blockIdx].getIndexFiles()[i];
        std::cout << "\"" << abs_path << "\"" << "\"" << cli->getServer().get_locations()[_blockIdx].getIndexFiles()[i] << "\""<< std::endl;
        if (stat(filePath.c_str(), &fileInfo) == 0 && S_ISREG(fileInfo.st_mode))
        {
            abs_path = abs_path + cli->getServer().get_locations()[_blockIdx].getIndexFiles()[i];
            return true;
        }
    }
    return false;
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
     std::string pathExact;
    if (_blockIdx == -1)
        pathExact = cli->getServer().get_root();
    else 
        pathExact = cli->getServer().get_locations()[_blockIdx].getRoot();
    // std::cout << cli->getServer().get_roots()[0] << std::endl;
    abs_path = pathExact + _path;
    std::cout << "abs_path: " << abs_path << std::endl;
    if (stat(abs_path.c_str(), &fileInfo) != 0) {
        std::string htmlfile = generateHTML_file("ERROR: NOT FOUND", 0, 404);
        cli->_response.setHeader("Content-Type", "text/html");
        cli->_response.setHeader("Content-Length", htmlfile.length());
        cli->_response.setBody(htmlfile);
        cli->_response.generateHeaderResponse();
        cli->setTypeData(WRITEDATA);
        std::cout << "\n\n\n\nERRRRRRRRRRRROR\n\n\n\n";
        cli->_response.setStatus(404);
        throw(404);
    }
}

const std::string RequestHandler::get_ressource_type(std::string abs_path)
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

size_t RequestHandler::getPathSize()
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







