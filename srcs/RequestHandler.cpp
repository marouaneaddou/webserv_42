// #include "../includes/RequestHandler.hpp"
// #include "../includes/RequestHandler.hpp"
#include "../includes/Client.hpp"


RequestHandler::RequestHandler(int &index, std::string &_path)
{
    this->_path = _path;
    _blockIdx = index;
    abs_path = "";
}


void RequestHandler::handleRequest(Client* cli)
{
    try
    {
        check_requested_method(cli);
    }
    catch (...)
    {
        setStatusMessage(cli);
        try
        {
            cli->_response.generate_ErrorPages(cli);
        }
        catch(int)
        {
            std::string htmlfile = cli->_response.generateHTML_file("ERROR NOT FOUND", 0, cli->_response.getStatus());
            cli->_response.setBody(htmlfile);
            cli->_response.setHeader("Content-Length", htmlfile.length());
            cli->_response.setHeader("Content-Type", "text/html");
        }
        cli->_response.generateHeaderResponse();
        cli->setTypeData(WRITEDATA);
    
    }
}

char **custom_cgi_envpGET(std::string abspath, std::string _path, std::string contentlength, std::string query)
{
    std::vector<std::string> envp_as_vec;

    envp_as_vec.push_back("CONTENT_TYPE=text/html");
    envp_as_vec.push_back("CONTENT_LENGTH=" + contentlength);
    envp_as_vec.push_back("SCRIPT_FILENAME=" + abspath);
    envp_as_vec.push_back("SCRIPT_NAME=" + _path);
    envp_as_vec.push_back("QUERY_STRING=" + query);
    envp_as_vec.push_back("REQUEST_METHOD=GET");
    envp_as_vec.push_back("REDIRECT_STATUS=200");
    envp_as_vec.push_back("SERVER_NAME=webserv");
    envp_as_vec.push_back("SERVER_PROTOCOL=HTTP/1.1");

    int envp_size = envp_as_vec.size();
    char **envp = new char*[envp_size + 1];

    for (int i = 0; i < envp_size; i++)
    {
        envp[i] = new char[envp_as_vec[i].size() + 1]; 
        strcpy(envp[i], envp_as_vec[i].c_str());
    }

    envp[envp_size] = nullptr;
    return envp;
}


std::string cgi_exec(std::string abspath, std::string _path, std::string contentlength, std::string query) {
    char **envp = custom_cgi_envpGET(abspath, _path, contentlength, query);
    std::string cgi_response;

    const char *av[3];
    av[0] = "/usr/local/bin/python3";
    av[1] = abspath.c_str();
    av[2] = NULL;
    std::string output_file = "/tmp/cgi_output.txt";

    pid_t pid = fork();
    if (pid == 0) {
        int fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd < 0) {
            std::cerr << "Failed to open output file in child process." << std::endl;
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);

        execve(av[0], const_cast<char **>(av), envp);
        exit(errno);
    } 
    else if (pid > 0) {
        const int timeout_seconds = 2;
        struct timeval start_time, current_time;
        gettimeofday(&start_time, NULL);

        while (true) {
            int status;
            pid_t result = waitpid(pid, &status, WNOHANG);
            
            if (result == pid) {
                // Child process has finished
                std::ifstream outfile(output_file.c_str());
                if (outfile.is_open()) {
                    std::string line;
                    while (std::getline(outfile, line)) {
                        cgi_response += line + "\n";
                    }
                    outfile.close();
                }
                break;
            } 
            else if (result == -1) {
                std::cerr << "waitpid error: " << hstrerror(errno) << std::endl;
                break;
            }

            gettimeofday(&current_time, NULL);
            double elapsed = (current_time.tv_sec - start_time.tv_sec) + 
                             (current_time.tv_usec - start_time.tv_usec) / 1000000.0;
            
            if (elapsed >= timeout_seconds) {
                kill(pid, SIGKILL);
                waitpid(pid, NULL, 0);
                cgi_response = "<h1>GET CGI script timed out.<h1/>\n";
                break;
            }

            usleep(10000);
        }
    }
    for (int i = 0; envp[i] != NULL; i++) {
        delete[] envp[i];
    }
    delete[] envp;
    remove(output_file.c_str());
    return cgi_response;
}

char ** custom_cgi_envpPOST(std::string abspath, std::string _path, std::string contentlength, std::string boundary, std::string uploaddir) {
    std::vector<std::string> envp_as_vec;

    envp_as_vec.push_back("CONTENT_TYPE=multipart/form-data; boundary=" + boundary);
    envp_as_vec.push_back("CONTENT_LENGTH=" + contentlength);
    envp_as_vec.push_back("SCRIPT_FILENAME=" + abspath);
    envp_as_vec.push_back("SCRIPT_NAME=" + _path);
    envp_as_vec.push_back("REQUEST_METHOD=POST");
    envp_as_vec.push_back("REDIRECT_STATUS=200");
    envp_as_vec.push_back("SERVER_NAME=webserv");
    envp_as_vec.push_back("SERVER_PROTOCOL=HTTP/1.1");
    envp_as_vec.push_back("UPLOAD_DIR=" + uploaddir);

    int envp_size = envp_as_vec.size();
    char **envp = new char*[envp_size + 1]; // Allocate array of pointers

    for (int i = 0; i < envp_size; i++) {
        envp[i] = new char[envp_as_vec[i].size() + 1]; // Allocate each string
        strcpy(envp[i], envp_as_vec[i].c_str());
    }

    envp[envp_size] = nullptr; // Null-terminate the array
    return envp;
}


std::string extract_boundary(const std::string &body) {
    size_t pos = body.find("--");
    if (pos != std::string::npos) {
        size_t end_pos = body.find("\r\n", pos);
        if (end_pos != std::string::npos) {
            return body.substr(pos + 2, end_pos - (pos + 2));
        }
    }
    return "";
}

std::string cgi_execPOST(std::string abspath, std::string _path, std::string contentlength, std::string body, std::string uploaddir) {
    std::string boundary = extract_boundary(body);
    char  **envp = custom_cgi_envpPOST(abspath, _path, contentlength, boundary, uploaddir);

    std::string cgi_response;
    const char *av[] = {"/usr/local/bin/python3", abspath.c_str(), nullptr};

    std::string input_file = "/tmp/cgi_input.txt";
    std::string output_file = "/tmp/cgi_output.txt";

    std::ofstream infile(input_file);
    if (!infile) {
        std::cerr << "Failed to open input file: " << hstrerror(errno) << std::endl;
        return "Internal Server Error";
    }
    infile << body;
    infile.close();

    pid_t pid = fork();
    if (pid == 0) {
        int in_fd = open(input_file.c_str(), O_RDONLY);
        int out_fd = open(output_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
        
        if (in_fd < 0 || out_fd < 0) {
            std::cerr << "Failed to open file in child process: " << hstrerror(errno) << std::endl;
            exit(1);
        }
        dup2(in_fd, STDIN_FILENO);
        dup2(out_fd, STDOUT_FILENO);
        close(in_fd);
        close(out_fd);
        execve(av[0], const_cast<char **>(av), envp);
        std::cerr << "execve failed: " << hstrerror(errno) << std::endl;
        exit(1);
    } 
    else if (pid > 0) {
        const int timeout_seconds = 5;
        struct timeval start_time, current_time;
        gettimeofday(&start_time, NULL);

        while (true) {
            int status;
            pid_t result = waitpid(pid, &status, WNOHANG);
            
            if (result == pid) {
                std::ifstream outfile(output_file);
                if (outfile) {
                    std::string line;
                    while (std::getline(outfile, line)) {
                        cgi_response += line + "\n";
                    }
                }
                break;
            } 
            else if (result == -1) {
                std::cerr << "waitpid error: " << hstrerror(errno) << std::endl;
                break;
            }
            gettimeofday(&current_time, NULL);
            double elapsed = (current_time.tv_sec - start_time.tv_sec) + 
                             (current_time.tv_usec - start_time.tv_usec) / 1000000.0;
            
            if (elapsed >= timeout_seconds) {
                kill(pid, SIGKILL);
                waitpid(pid, NULL, 0);
                cgi_response = "CGI script timed out.\n";
                break;
            }

            usleep(10000);
        }
    }
    else {
        std::cerr << "Fork failed: " << hstrerror(errno) << std::endl;
        return "Internal Server Error";
    }

    if (unlink(input_file.c_str()) != 0 || unlink(output_file.c_str()) != 0) {
        std::cerr << "Failed to remove temporary files: " << hstrerror(errno) << std::endl;
    }
    for (int i = 0; envp[i] != NULL; i++) {
        delete[] envp[i];
    }
    delete[] envp;
    return cgi_response;
}



void RequestHandler::check_requested_method(Client* cli)
{
    std::string cgi_response;
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
                    if (cli->getServer().get_locations()[_blockIdx].getDirectoryListing() == false)
                    {
                        cli->_response.setStatus(403);
                        cli->_response.setHeader("Content-Length", 0);
                        throw(403);
                    }
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
            int pos = cli->_request.getURL().find("?");
            std::string query = cli->_request.getURL().substr(pos + 1);
            cgi_response = cgi_exec(abs_path, _path, std::to_string(cli->_request.getHeader("Content-Length")->second.size()), query);
            cli->_response.setStatus(200);
            std::string htmlfile = cgi_response;
            cli->_response.setHeader("Content-Type", "text/html");
            cli->_response.setHeader("Content-Length", htmlfile.length());
            cli->_response.setBody(htmlfile);
            cli->_response.generateHeaderResponse();
            cli->setTypeData(WRITEDATA);
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
            if (get_ressource_type(abs_path) == "DIR")
            {
                if (_path[_path.length() - 1] != '/')
                {
                    if (access(abs_path.c_str(), R_OK) != 0) 
                    {
                        std::string htmlfile = cli->_response.generateHTML_file("ERROR: forbiden access", 0, 403);
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
                    throw(307);
                }
                if (is_dir_has_index_files(cli) == true) 
                {
                    if (access(abs_path.c_str(), R_OK) != 0) 
                    {
                        
                        std::string htmlfile = cli->_response.generateHTML_file("ERROR: forbiden access", 0, 403);
                        cli->_response.setHeader("Content-Type", "text/html");
                        cli->_response.setHeader("Content-Length", htmlfile.length());
                        cli->_response.setBody(htmlfile);
                        cli->_response.generateHeaderResponse();
                        cli->_response.setStatus(403);
                        cli->setTypeData(WRITEDATA);
                        throw(403);
                    }
                    if (cli->getServer().get_locations()[_blockIdx].getCgiSupport() == 1)  
                    {
                        std::string uploaddir = cli->getServer().get_locations()[_blockIdx].getUploadDir();
                        cgi_response = cgi_execPOST(abs_path, _path, 
                                std::to_string(cli->_request.getHeader("Content-Length")->second.size()),
                                cli->_request.getBody(), uploaddir);
                        cli->_response.setStatus(200);
                        std::string htmlfile =  cli->_response.generateHTML_file("The file upload was successful.", 1, 200);
                        cli->_response.setHeader("Content-Type", "text/html");
                        cli->_response.setHeader("Content-Length", htmlfile.length());
                        cli->_response.setBody(htmlfile);
                        cli->_response.generateHeaderResponse();
                        cli->setTypeData(WRITEDATA);
                    }
                    else 
                    {
                        std::string htmlfile =  cli->_response.generateHTML_file("ERROR: NOT Support CGI", 0, 403);
                        cli->_response.setStatus(403);
                        cli->_response.setBody(htmlfile);
                        cli->_response.setHeader("Content-Type", "text/html");
                        cli->_response.setHeader("Content-Length", htmlfile.length());
                        cli->_response.generateHeaderResponse();
                        cli->setTypeData(WRITEDATA);
                        throw(403);
                    }
                }
                else 
                {    
                    std::string htmlfile =  cli->_response.generateHTML_file("ERROR: NOT index file", 0, 403);
                    cli->_response.setBody(htmlfile);
                    cli->_response.setHeader("Content-Type", "text/html");
                    cli->_response.setHeader("Content-Length", htmlfile.length());
                    cli->_response.setStatus(403);
                    cli->_response.generateHeaderResponse();
                    cli->setTypeData(WRITEDATA);
                    throw(403);
                } 
            }
            else
            {
                 if (access(abs_path.c_str(), R_OK) != 0) 
                 {
                    std::string htmlfile =  cli->_response.generateHTML_file("ERROR: forbiden access", 0, 403);
                    cli->_response.setHeader("Content-Type", "text/html");
                    cli->_response.setHeader("Content-Length", htmlfile.length());
                    cli->_response.setBody(htmlfile);
                    cli->_response.generateHeaderResponse();
                    cli->_response.setStatus(403);
                    cli->setTypeData(WRITEDATA);
                    throw(403);
                }
                    if (cli->getServer().get_locations()[_blockIdx].getCgiSupport() == 1)  {
                                        std::string uploaddir = cli->getServer().get_locations()[_blockIdx].getUploadDir();
                                        cgi_response = cgi_execPOST(abs_path, _path, 
                                                std::to_string(cli->_request.getHeader("Content-Length")->second.size()),
                                                cli->_request.getBody(), uploaddir); 
                                        cli->_response.setStatus(200);
                                         std::string htmlfile =  cli->_response.generateHTML_file("The file upload was successful.", 1, 200);

                                        cli->_response.setHeader("Content-Type", "text/html");
                                        cli->_response.setHeader("Content-Length", htmlfile.length());
                                        cli->_response.setBody(htmlfile);
                                        cli->_response.generateHeaderResponse();
                                        cli->setTypeData(WRITEDATA);
                    }
                    else {
                        std::string htmlfile =  cli->_response.generateHTML_file("ERROR: NOT seport CGI", 0, 403);
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
                    cgi_response = cgi_exec(abs_path, _path, std::to_string(cli->_request.getHeader("Content-Length")->second.size()), "");
                else
                {

                    cli->_response.setStatus(403);
                    throw(403);
                }
            }
        }
        else if (get_ressource_type(abs_path) == "FILE") {
            if(cli->getServer().get_locations()[_blockIdx].getCgiSupport() == 1)
                cgi_response = cgi_exec(abs_path, _path, std::to_string(cli->_request.getHeader("Content-Length")->second.size()), "");
            else 
                handleDeleteRequest(cli, abs_path);
        }
        if (get_ressource_type(abs_path) == "DIR" && cli->getServer().get_locations()[_blockIdx].getCgiSupport() == 0)
            handleDeleteRequest(cli, abs_path);
        if (cli->getServer().get_locations()[_blockIdx].getCgiSupport() != 1){
            cli->_response.setStatus(204);
            throw(204);
        }
        cli->_response.setStatus(200);  
        std::string htmlfile = cgi_response;
        cli->_response.setHeader("Content-Type", "text/html");
        cli->_response.setHeader("Content-Length", htmlfile.length());
        cli->_response.setBody(htmlfile);
        cli->_response.generateHeaderResponse();
        cli->setTypeData(WRITEDATA);
    }
    return;
}

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
        deleteDirectoryRecursively(cli, abs_path.c_str());
    else
        std::remove(abs_path.c_str());  
}



bool RequestHandler::is_dir_has_index_files(Client* cli)
{
    struct stat fileInfo;
    for (unsigned int i = 0; i < cli->getServer().get_locations()[_blockIdx].getIndexFiles().size(); i++)
    {
        std::string filePath = abs_path + cli->getServer().get_locations()[_blockIdx].getIndexFiles()[i];
        if (stat(filePath.c_str(), &fileInfo) == 0 && S_ISREG(fileInfo.st_mode))
        {
            abs_path = abs_path + cli->getServer().get_locations()[_blockIdx].getIndexFiles()[i];
            return true;
        }
    }
    return false;
}


void RequestHandler::get_requested_ressource(Client* cli)
{
    struct stat fileInfo;
    std::string htmlfile;
    std::string url = cli->_request.getURL();
    std::size_t query_pos = url.find("?");
    
    if (query_pos != std::string::npos)
        _path = url.substr(0, query_pos);
    else
        _path = url;
        
    std::string pathExact;
    pathExact = cli->getServer().get_locations()[_blockIdx].getRoot();
    abs_path = pathExact + _path;
    if (stat(abs_path.c_str(), &fileInfo) != 0) {
        htmlfile =  cli->_response.generateHTML_file("ERROR: NOT FOUND", 0, 404);
        cli->_response.setBody(htmlfile);  
        cli->_response.setHeader("Content-Type", "text/html");
        cli->_response.setHeader("Content-Length", htmlfile.length());
        cli->setTypeData(WRITEDATA);
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







