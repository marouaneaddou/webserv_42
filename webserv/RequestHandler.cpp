#include "RequestHandler.hpp"


RequestHandler::RequestHandler()
{}

void RequestHandler::req_uri_location(Client* cli)
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
        if (cli->getServer().locations[_blockIdx].getAcceptedMethod()[i] == cli->_request.getMethod())
            return;
    }
    cli->_response.setStatus(405);
    throw(405);
}


// void check_execution(std::string cgi_path, struct stat *file_info)
// {

//     //if cgi_extension exist conf and path_python exist and url ends with cgi extens
    
//     if (stat(cgi_path.c_str(), file_info) == -1)
//         throw (7);
//     if (file_info->)
//     //fill av execve
//     char **av;
//     av = malloc(3*sizeof(char *));
//     av[0] = malloc("/usr/local/bin/python3".size());
//     av[0] = scrcpy("/usr/local/bin/python3");
//     av[1] = malloc(cgi_path.size());
//     av[1] = strcpy(cgi_path.c_str());
//     av[2] = NULL;
//     //fill envp execve
//     void    HttpRequestHandler::add_standard_CGI_environment_variable()
//     {
//         std::map<std::string, std::string>::iterator it = request_headers.find("Content-Length");
//         if (it != request_headers.end())
//             envp_as_vec.push_back("CONTENT_LENGTH=" + it->second);
//         it = request_headers.find("Content-Type");
//         if (it != request_headers.end())
//             envp_as_vec.push_back("CONTENT_TYPE=" + it->second);
//         envp_as_vec.push_back("SCRIPT_FILENAME=" + requested_resource_path);
//         envp_as_vec.push_back("SCRIPT_NAME=" +  request_url_as_vector.back());
//         envp_as_vec.push_back("QUERY_STRING=");
//         envp_as_vec.push_back("REQUEST_METHOD=" + request_method);
//         envp_as_vec.push_back("REDIRECT_STATUS=200");
//         envp_as_vec.push_back("SERVER_NAME=webserv");
//         envp_as_vec.push_back("SERVER_PROTOCOL=HTTP/1.1");
//     }
//     //convert envp_as_vec to c style 
//     //change status
// }



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
            std::cout << buffer << std::endl;
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
                    std::cout << "directory in GET\n";
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
                    if (cli->getServer().locations[_blockIdx].directory_listing == false)
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
        if (cli->getServer().locations[_blockIdx].getCgiSupport() == 1 && cli->checkExtensionFile(abs_path)) {
             /*****************************************/
            /***************CGI CGI CGI !!!!!!!!!!!!!*****************/
            // std::cout << "@@ PATH" << _path << std::endl;
            // exit(0);
            std::string cgi_response = cgi_exec(abs_path, 1);

                        


            /*****************************************/

            // check return cgi !!!!!!!!!!
            /*****this just test ====>***/ cli->_response.setStatus(200);

            /****************************************/
                            // std::string htmlfile = "<!DOCTYPE html>\n"
                            //                     "<html lang=\"en\">\n"
                            //                     "<head>\n"
                            //                     "    <meta charset=\"UTF-8\">\n"
                            //                     "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                            //                     "    <title>Document</title>\n"
                            //                     "</head>\n"
                            //                     "<body>\n"
                            //                     "    <div>\n"
                            //                     "        <p style=\"color: brown; font-size: 35px;\">CGI GET method.</p>\n"
                            //                     "    </div>\n"
                            //                     "</body>\n"
                            //                     "</html>\n";
                        std::string htmlfile = cgi_response;
                        cli->_response.setHeader("Content-Type", "text/html");
                        cli->_response.setHeader("Content-Length", htmlfile.length());
                        cli->_response.setBody(htmlfile);
                        cli->_response.generateHeaderResponse();
            cli->setTypeData(WRITEDATA);
            /****************************************/
            // throw(200);
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
        
            get_requested_ressource(cli);
            cli->setOnetime();
            if (get_ressource_type(abs_path) == "DIR") {
                if (_path[_path.length() - 1] != '/') {
                    if (access(abs_path.c_str(), R_OK) != 0) {
                        std::cout << "NOT access\n";
                        std::string htmlfile = "<!DOCTYPE html>\n"
                                                            "<html lang=\"en\">\n"
                                                            "<head>\n"
                                                            "    <meta charset=\"UTF-8\">\n"
                                                            "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                                                            "    <title>Document</title>\n"
                                                            "</head>\n"
                                                            "<body>\n"
                                                            "    <div>\n"
                                                            "        <p style=\"color: brown; font-size: 35px;\">ERROR</p>\n"
                                                            "    </div>\n"
                                                            "</body>\n"
                                                            "</html>\n";
                        cli->_response.setHeader("Content-Type", "text/html");
                        cli->_response.setHeader("Content-Length", htmlfile.length());
                        cli->_response.setBody(htmlfile);
                        cli->_response.generateHeaderResponse();
                        cli->_response.setStatus(403);
                        cli->setTypeData(WRITEDATA);
                        throw(403);
                    // cli->_response.setHeader("Location", _path + '/');
                    // cli->_response.setStatus(307);
                    // throw(301);
                    }
                }
            
                if (is_dir_has_index_files(cli) == true) {

                    if (access(abs_path.c_str(), R_OK) != 0) {
                        std::cout << "NOT access\n";
                        std::string htmlfile = "<!DOCTYPE html>\n"
                                                            "<html lang=\"en\">\n"
                                                            "<head>\n"
                                                            "    <meta charset=\"UTF-8\">\n"
                                                            "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                                                            "    <title>Document</title>\n"
                                                            "</head>\n"
                                                            "<body>\n"
                                                            "    <div>\n"
                                                            "        <p style=\"color: brown; font-size: 35px;\">ERROR</p>\n"
                                                            "    </div>\n"
                                                            "</body>\n"
                                                            "</html>\n";
                        cli->_response.setHeader("Content-Type", "text/html");
                        cli->_response.setHeader("Content-Length", htmlfile.length());
                        cli->_response.setBody(htmlfile);
                        cli->_response.generateHeaderResponse();
                        cli->_response.setStatus(403);
                        cli->setTypeData(WRITEDATA);
                        throw(403);
                    }
                    // check location have or support uploud 
                    if (cli->getServer().locations[_blockIdx].getCgiSupport() == 1)  {
                                        /*****************************************/


                                                    /***************CGI CGI CGI !!!!!!!!!!!!!*****************/


                                        /*****************************************/

                                        // check return cgi !!!!!!!!!!
                                        /*****this just test ====>***/ cli->_response.setStatus(200);

                                        /****************************************/
                                        std::string htmlfile = "<!DOCTYPE html>\n"
                                                            "<html lang=\"en\">\n"
                                                            "<head>\n"
                                                            "    <meta charset=\"UTF-8\">\n"
                                                            "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                                                            "    <title>Document</title>\n"
                                                            "</head>\n"
                                                            "<body>\n"
                                                            "    <div>\n"
                                                            "        <p style=\"color: brown; font-size: 35px;\">The file upload was successful.</p>\n"
                                                            "    </div>\n"
                                                            "</body>\n"
                                                            "</html>\n";

                                        cli->_response.setHeader("Content-Type", "text/html");
                                        cli->_response.setHeader("Content-Length", htmlfile.length());
                                        cli->_response.setBody(htmlfile);
                                        cli->_response.generateHeaderResponse();
                                        cli->setTypeData(WRITEDATA);
                        /****************************************/
                        // throw(200);
                    }
                    else {
                        std::string htmlfile = "<!DOCTYPE html>\n"
                                                            "<html lang=\"en\">\n"
                                                            "<head>\n"
                                                            "    <meta charset=\"UTF-8\">\n"
                                                            "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                                                            "    <title>Document</title>\n"
                                                            "</head>\n"
                                                            "<body>\n"
                                                            "    <div>\n"
                                                            "        <p style=\"color: brown; font-size: 35px;\">ERROR</p>\n"
                                                            "    </div>\n"
                                                            "</body>\n"
                                                            "</html>\n";
                        cli->_response.setStatus(403);
                        cli->_response.setBody(htmlfile);
                        cli->_response.setHeader("Content-Type", "text/html");
                        cli->_response.setHeader("Content-Length", htmlfile.length());
                        cli->_response.generateHeaderResponse();
                        cli->setTypeData(WRITEDATA);
                        std::cout << "return responce if server not support cgi\n";
                        throw(403);
                    }
                }
                else {
                    cli->_response.setStatus(403);
                    cli->_response.generateHeaderResponse();
                    cli->setTypeData(WRITEDATA);
                    throw(403);
                } 
            }
            else {
                 if (access(abs_path.c_str(), R_OK) != 0) {
                        std::cout << "NOT access\n";
                        std::string htmlfile = "<!DOCTYPE html>\n"
                                                            "<html lang=\"en\">\n"
                                                            "<head>\n"
                                                            "    <meta charset=\"UTF-8\">\n"
                                                            "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                                                            "    <title>Document</title>\n"
                                                            "</head>\n"
                                                            "<body>\n"
                                                            "    <div>\n"
                                                            "        <p style=\"color: brown; font-size: 35px;\">ERROR</p>\n"
                                                            "    </div>\n"
                                                            "</body>\n"
                                                            "</html>\n";
                        cli->_response.setHeader("Content-Type", "text/html");
                        cli->_response.setHeader("Content-Length", htmlfile.length());
                        cli->_response.setBody(htmlfile);
                        cli->_response.generateHeaderResponse();
                        cli->_response.setStatus(403);
                        cli->setTypeData(WRITEDATA);
                        throw(403);
                    }
                    // check location have or support uploud 
                    if (cli->getServer().locations[_blockIdx].getCgiSupport() == 1)  {
                                        /*****************************************/


                                                    /***************CGI CGI CGI !!!!!!!!!!!!!*****************/


                                        /*****************************************/

                                        // check return cgi !!!!!!!!!!
                                        /*****this just test ====>***/ cli->_response.setStatus(200);

                                        /****************************************/
                                        std::string htmlfile = "<!DOCTYPE html>\n"
                                                            "<html lang=\"en\">\n"
                                                            "<head>\n"
                                                            "    <meta charset=\"UTF-8\">\n"
                                                            "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                                                            "    <title>Document</title>\n"
                                                            "</head>\n"
                                                            "<body>\n"
                                                            "    <div>\n"
                                                            "        <p style=\"color: brown; font-size: 35px;\">The file upload was successful.</p>\n"
                                                            "    </div>\n"
                                                            "</body>\n"
                                                            "</html>\n";

                                        cli->_response.setHeader("Content-Type", "text/html");
                                        cli->_response.setHeader("Content-Length", htmlfile.length());
                                        cli->_response.setBody(htmlfile);
                                        cli->_response.generateHeaderResponse();
                                        cli->setTypeData(WRITEDATA);
                        /****************************************/
                        // throw(200);
                    }
                    else {
                        std::string htmlfile = "<!DOCTYPE html>\n"
                                                            "<html lang=\"en\">\n"
                                                            "<head>\n"
                                                            "    <meta charset=\"UTF-8\">\n"
                                                            "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
                                                            "    <title>Document</title>\n"
                                                            "</head>\n"
                                                            "<body>\n"
                                                            "    <div>\n"
                                                            "        <p style=\"color: brown; font-size: 35px;\">ERROR</p>\n"
                                                            "    </div>\n"
                                                            "</body>\n"
                                                            "</html>\n";
                        cli->_response.setStatus(403);
                        cli->_response.setBody(htmlfile);
                        cli->_response.setHeader("Content-Type", "text/html");
                        cli->_response.setHeader("Content-Length", htmlfile.length());
                        cli->_response.generateHeaderResponse();
                        cli->setTypeData(WRITEDATA);
                        std::cout << "return responce if server not support cgi\n";
                        throw(403);
                    }
            }



        

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
        if(cli->getServer().locations[_blockIdx].getCgiSupport() == 1)
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
        if(cli->getServer().locations[_blockIdx].getCgiSupport() == 1)
            std::string cgi_response = cgi_exec(abs_path, 1);
        else
            std::remove(abs_path.c_str());  
    }
}



bool RequestHandler::is_dir_has_index_files(Client* cli)
{
    struct stat fileInfo;

    for (int i = 0; i < cli->getServer()._indexFiles.size(); i++)
    {
        std::string filePath = abs_path + cli->getServer()._indexFiles[i];
        if (stat(filePath.c_str(), &fileInfo) == 0 && S_ISREG(fileInfo.st_mode))
        {
            abs_path = abs_path + cli->getServer()._indexFiles[i];
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
    abs_path = cli->getServer().roots[0] + _path;
    if (stat(abs_path.c_str(), &fileInfo) != 0) {
        cli->_response.setStatus(404);
        //cli->setSizeFile(0);
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







