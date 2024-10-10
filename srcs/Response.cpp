
#include "../includes/Response.hpp"
#include "../includes/Client.hpp"

Response::Response()
{
    setStatus(200);
    setStatusMsg("OK");
}

Response::~Response()
{
    
}
it_Header Response::getHeader(const std::string& key) 
{
        return _headers.find(key);
}
    /*************** Status ************/

void Response::setStatus(const int &status)
{
    _status = status;
}

void Response::setStatusMsg(const std::string& msg)
{
    this->_Msg = msg;
}

int Response::getStatus() const
{
    return _status;
}

std::string Response::getStatusMsg() const
{
    return _Msg;
}
    /*************** Status ************/

    /*************** Body ************/

void Response::setBody(const std::string& body)
{
    _body = body;
}

void Response::setAppendBody(const char *buffer)
{
    _body.append(buffer);
}

std::string Response::getBody() const
{
    return _body;
}


    /*************** Body ************/

void Response::generateHeaderResponse() 
{
    _Response = "HTTP/1.1 " + std::to_string(getStatus()) + ' ' + getStatusMsg() + "\r\n";
    std::map<std::string, std::string>::iterator it =  _headers.begin();
    while (it != _headers.end())
    {
        _Response += it->first + ": " + it->second;
        if (++it != _headers.end()) {
            _Response += "\r\n";
        }
    }
    _Response += "\r\n\r\n";
    _Response += _body;
}


void Response::generateErrorPage(std::string root, std::string error_page)
{
    _body.clear();
    std::string path = root + error_page;
    int fd = open(path.c_str(), O_RDONLY);
    if (fd < 0)
    {
        perror("Error1");
        setStatus(404);
        setStatusMsg("NOT FOUND");
        throw(404);
    }
    char str[1000000];
    int n = read(fd, str, 1000000);
    if (n < 0)
    {
        perror("Erro2");
        exit(1);
    }
    str[n] = '\0';
    _body += str;
    setBody(_body);
    setHeader("Content-Length", _body.size());
}

  
  
  
std::string Response::generateHTML_file(std::string print, bool type, int status) {
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
                   "            display: flex;;\n"
                   "            justify-content: center;;\n"
                   "            align-items: center;;\n"
                   "            flex-direction: column;;\n"
                   "        }\n"
                   "        .container p {\n"
                   "            color: brown;\n"
                   "            text-align: center;\n"
                   "        }\n"
                   "    </style>\n"
                   "</head>\n"
                   "<body>\n"
                   "    <div class=\"container\">\n";
                   if (type == 0) {
                   html += "        <p style=\"font-size: 80px; color: brown;\">Oops !</p>\n";
                   html += "        <p style=\"font-size: 30px; color: brown; \"><span style=\"font-size: 50px;\">"+ std::to_string(status) + "</span> : " + print + "</p>\n";
                   }
                   else
                   {
                        html += "        <p style=\"font-size: 30px; color: green \"><span style=\"font-size: 50px;\">"+ std::to_string(status) + "</span> : " + print + "</p>\n";
                   }

                    html += "    </div>\n";
                    html += "</body>\n";
                    html += "</html>";
    return html;

}


void Response::generate_ErrorPages(Client *cli)
{
    std::map<std::string, std::string> it_error = cli->getServer().get_error_pages();
    if(it_error.find(std::to_string(cli->_response.getStatus())) != it_error.end())
    {
        std::string error_page = it_error.find(std::to_string(cli->_response.getStatus()))->second;
        cli->_response.generateErrorPage(cli->getServer().get_root(), error_page);
    }
    else
    {
        std::string htmlfile;
        if (cli->_response.getStatus() == 204)
            htmlfile = cli->_response.generateHTML_file("The file was deleted successfully", 1, cli->_response.getStatus());
        else
            htmlfile = cli->_response.generateHTML_file(cli->_response.getStatusMsg(), 0, cli->_response.getStatus());
        cli->_response.setBody(htmlfile);
        cli->_response.setHeader("Content-Length", htmlfile.length());
    }
    cli->_response.setHeader("Content-Type", "text/html");
}

void Response::Send(int cli_fd)
{
    signal(SIGPIPE, SIG_IGN);
    int read;
    if (_Response.size() > 1000000) read = 1000000;
    else read = _Response.size();
    int nbyte = send(cli_fd, _Response.data(), read, 0);
    if (nbyte <= 0)
    {
       std::cerr << "Error in send()" << std::endl;
    }
    _Response.erase(0, nbyte);
}
