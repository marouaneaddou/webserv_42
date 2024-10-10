
#include "../includes/WebServer.hpp"

WebServ::WebServ(){
    this->max_fd = 0;
    this->_exactLocation = -1;
}

WebServ::~WebServ()
{
    for (unsigned int i = 0; i < _servers.size(); i++)
        delete _servers[i];
    for (itClient it_cli = _clients.begin(); it_cli != _clients.end(); it_cli++)
    {
        delete it_cli->second;
    }
    // delete handler;
    _servers.clear();
    _clients.clear();
    servers_fds.clear();
    clients_fds.clear();
    _myconf.clear();
}

void WebServ::run_servers(std::map<int, std::vector<Servers> > grouped, std::vector<Servers> confs)
{
    for (unsigned int i = 0; i <  confs.size(); i++)
    {
        _servers.push_back(new TCPserver(confs[i]));
    }
    FD_ZERO(&current_Rsockets);
    FD_ZERO(&current_Wsockets);
    SetListeners();
    while (true)
    {
        ready_Rsockets = current_Rsockets;
        ready_Wsockets = current_Wsockets;
        int selectResult = select(max_fd + 1, &ready_Rsockets, &ready_Wsockets, NULL, NULL);
        if (selectResult <= 0) {
            std::cerr << "Error in select(): " << strerror(errno) << std::endl;
            exit(EXIT_FAILURE);
        }

        for (unsigned int idx = 0; idx < servers_fds.size(); idx++)
        {
            if (FD_ISSET(servers_fds[idx], &ready_Rsockets))
            {
                int new_socket = 0;
                socklen_t _sockaddr_len = sizeof(_sockaddr);
                if ((new_socket = accept(servers_fds[idx], (sockaddr *)&_sockaddr,  &_sockaddr_len)) < 0){
                    perror("Server failed to accept incoming connection");
                    continue;
                }
                if (set_non_blocking(new_socket) < 0) {
                    std::cerr << "Failed to set non-blocking for socket " << new_socket << ": " << strerror(errno) << std::endl;
                    close(new_socket);
                    continue;
                }
                std::cout << "\033[32mNew connection accepted on clientFD: " << new_socket << "\033[0m" << std::endl;
                Client *newClient = new Client(new_socket);
                _clients.insert(std::make_pair(new_socket, newClient));
                FD_SET(new_socket, &current_Rsockets);
                if (new_socket > this->max_fd)
                {
                    this->max_fd = new_socket;
                }
            }
        }
        
        for (itClient it_cli = _clients.begin();  it_cli != _clients.end();)
        {
            if (FD_ISSET(it_cli->first, &ready_Rsockets))
            {
                int check = read_request(it_cli->first);
                if (check == -1)
                {
                    itClient temp = it_cli;
                    ++it_cli;
                    delete temp->second;
                    _clients.erase(temp);
                    continue;
                }
                try
                {
                    start_parsing(it_cli->first, grouped);
                    if (_clients[it_cli->first]->getTypeData() != WRITEDATA) 
                    {
                        if ( _clients[it_cli->first]->_request.getHeader("Transfer-Encoding") != _clients[it_cli->first]->_request.getEndHeaders())
                        {
                            if (_clients[it_cli->first]->_buffer.find("0\r\n\r\n") != std::string::npos)
                            {
                                _clients[it_cli->first]->_request.findTypeOfPostMethod();
                                _clients[it_cli->first]->_request.setBody(_clients[it_cli->first]->_buffer);
                                _clients[it_cli->first]->_request.removeHexaDecimalInBody();
                                if (_clients[it_cli->first]->getServer().get_client_body_size() < _clients[it_cli->first]->_request.getBody().size()) {
                                    _clients[it_cli->first]->_response.setStatus(413);
                                    _clients[it_cli->first]->_response.setStatusMsg("Payload Too Large");
                                    _clients[it_cli->first]->_buffer.clear();
                                    throw(413);
                                }
                                _clients[it_cli->first]->_buffer.clear();
                                FD_CLR(it_cli->first, &current_Rsockets);
                                FD_SET(it_cli->first, &current_Wsockets);
                            }
                        }
                        else if (_clients[it_cli->first]->_request.getHeader("Content-Length")  != _clients[it_cli->first]->_request.getEndHeaders())
                        {
                            if (_clients[it_cli->first]->_buffer.size() == static_cast<size_t>(stoi(_clients[it_cli->first]->_request.getHeader("Content-Length")->second)))
                            {
                                _clients[it_cli->first]->_request.findTypeOfPostMethod();
                                _clients[it_cli->first]->_request.setBody(_clients[it_cli->first]->_buffer);
                                if (_clients[it_cli->first]->getServer().get_client_body_size() < _clients[it_cli->first]->_request.getBody().size()) {
                                    _clients[it_cli->first]->_response.setStatus(413);
                                    _clients[it_cli->first]->_response.setStatusMsg("Payload Too Large");
                                    _clients[it_cli->first]->_buffer.clear();
                                    throw(413);
                                }
                                _clients[it_cli->first]->_buffer.clear();
                                FD_CLR(it_cli->first, &current_Rsockets);
                                FD_SET(it_cli->first, &current_Wsockets);
                            }
                        }
                    }
                }
                catch(int status) 
                {
                    _clients.at(it_cli->first)->_response.generate_ErrorPages(_clients.at(it_cli->first));
                    _clients[it_cli->first]->_response.generateHeaderResponse();
                    FD_CLR(it_cli->first, &current_Rsockets);
                    FD_SET(it_cli->first, &current_Wsockets);
                    _clients.at(it_cli->first)->setTypeData(WRITEDATA);
                }
            }
            if (FD_ISSET(it_cli->first, &ready_Wsockets))
            {
                if (_clients.at(it_cli->first)->getOnetime() == false && _clients.at(it_cli->first)->_response.getStatus() == 200) {
                    _clients.at(it_cli->first)->handler =  RequestHandler(_exactLocation, _path);
                }
                if (_clients.at(it_cli->first)->_response.getStatus() == 200)
                {
                    _clients.at(it_cli->first)->handler.handleRequest(_clients.at(it_cli->first));
                }
                if (_clients.at(it_cli->first)->getTypeData() == WRITEDATA)
                {
                    _clients.at(it_cli->first)->_response.Send(it_cli->first);
                    if ( _clients.at(it_cli->first)->_response.getResponse().size() == 0) {
                        _clients.at(it_cli->first)->setTypeData(CLOSESOCKET);
                    }
                }
                if (_clients.at(it_cli->first)->getTypeData() == CLOSESOCKET)
                {
                    FD_CLR(it_cli->first, &current_Wsockets);
                    close(it_cli->first);
                    delete it_cli->second;
                    it_cli = _clients.erase(it_cli);
                    continue;
                }
            }
            ++it_cli;
        }
    }
}


int WebServ::read_request(int fd_R)
{
    if ((_nbytes = recv(fd_R, _buf, 25000, 0)) <= 0)
    {
        std::cerr << "Error in recv()" << std::endl;
        close(fd_R);
        FD_CLR(fd_R, &current_Rsockets);
        return (-1);
    }
    if (_nbytes < static_cast<int>(sizeof(_buf)))
        _buf[_nbytes] = '\0';
    _clients[fd_R]->_buffer.append(_buf, _nbytes);
    return (0);
}

void WebServ::start_parsing(int fd_R, std::map<int, std::vector<Servers> > grouped)
{
    if (_clients.at(fd_R)->getCheck() == false &&_clients[fd_R]->_buffer.find("\r\n\r\n") != std::string::npos)
    {
        int findPos = _clients[fd_R]->_buffer.find("\r\n");
        _clients.at(fd_R)->setCheck();
        _firstline = _clients[fd_R]->_buffer.substr(0, findPos);
        _clients.at(fd_R)->_request.parse_request_line(_firstline);
        _clients[fd_R]->_buffer = _clients[fd_R]->_buffer.substr(findPos + 2);
        if (_clients.at(fd_R)->_request.getMethod() != "POST")
        {
            _clients.at(fd_R)->_request.setHeader(_clients[fd_R]->_buffer);
            Servers server = getConf(fd_R, grouped,  _clients[fd_R]->_request.getHeader("Host")->second);
            _clients[fd_R]->setConf(server);
            _clients[fd_R]->_request.isReqWellFormed(_clients[fd_R]->getResponse());
            req_uri_location(_clients[fd_R]);
            if (is_location_have_redirection(_clients[fd_R]) == 0)
                is_method_allowed_in_location(_clients[fd_R]);
            else 
                _clients[fd_R]->_response.generateHeaderResponse();
            _clients.at(fd_R)->setTypeData(WRITEDATA);
            FD_CLR(fd_R, &current_Rsockets);
            FD_SET(fd_R, &current_Wsockets);
        }
        else
        {
            findPos = _clients[fd_R]->_buffer.find("\r\n\r\n");
            _clients.at(fd_R)->_request.setHeader(_clients[fd_R]->_buffer.substr(0, findPos + 2));
            Servers server = getConf(fd_R, grouped,  _clients[fd_R]->_request.getHeader("Host")->second);
            _clients[fd_R]->setConf(server);
            _clients[fd_R]->_request.isReqWellFormed(_clients[fd_R]->getResponse());
            req_uri_location(_clients[fd_R]);
            if (is_location_have_redirection(_clients[fd_R]) == 0)
                is_method_allowed_in_location(_clients[fd_R]);
            else if (is_location_have_redirection(_clients[fd_R]) == 1)
            {
                _clients[fd_R]->_response.generateHeaderResponse();
                _clients.at(fd_R)->setTypeData(WRITEDATA);
                FD_CLR(fd_R, &current_Rsockets);
                FD_SET(fd_R, &current_Wsockets);
            }
            _clients[fd_R]->_buffer = _clients[fd_R]->_buffer.substr(findPos + 4);
            _clients.at(fd_R)->setTypeData(READDATA);
        }
    }

}

void WebServ::SetListeners()
{
    for (unsigned int i = 0; i < _servers.size(); i++)
    {
        for (unsigned int j = 0; j < _servers[i]->getSocket().size(); j++)
        {
            servers_fds.push_back(_servers[i]->getSocket()[j]);
            FD_SET(_servers[i]->getSocket()[j], &current_Rsockets);
            if (_servers[i]->getSocket()[j] > this->max_fd)
                max_fd = _servers[i]->getSocket()[j];
        }
    }
}

int WebServ::set_non_blocking(int sock) 
{
    if (fcntl(sock, F_SETFL, O_NONBLOCK) == -1) 
    {
        perror("fcntl F_SETFL");
        return(-1);
    }
    return (0);
}

Servers WebServ::getConf(int fd, std::map<int, std::vector<Servers> > confs, std::string host)
{
    (void)fd;
    int twoPoint = host.find(":");
    std::string ip = host.substr(0, twoPoint);
    int port = stoi(host.substr(twoPoint + 1));
    
    std::vector<Servers> servers = confs[port];
    for (size_t i = 0; i < servers.size(); i++) {
        if (servers[i].get_host() == ip){
            return servers[i];
        }
    }
    for (size_t i = 0; i < servers.size(); i++) {
        std::vector<std::string> names = servers[i].get_server_names();
        for (size_t j = 0; j < names.size(); j++) {
            if (names[j] == ip) 
            {
                return servers[i];
            }
        }
    }
    _clients[fd]->_response.setStatus(404);
    _clients[fd]->_response.setStatusMsg("NOT FOUND");
    throw(404);
}

void WebServ::req_uri_location(Client* cli)
{
    std::string url = cli->_request.getURL();
    std::size_t query_pos = url.find("?");
    if (query_pos != std::string::npos)
        _path = url.substr(0, query_pos);
    else
        _path = url;
    std::string path = url.substr(0, query_pos);

    for (unsigned int i = 0; i < cli->getServer().get_locations().size(); i++)
    {
        if (cli->getServer().get_locations()[i].getPath()[0] == '=' && cli->getServer().get_locations()[i].getPath().substr(2) == _path)
        {
            _exactLocation = i;
            return ;
        }
    }
    std::string longestMatch;
    for (unsigned int i = 0; i < cli->getServer().get_locations().size(); i++)
    {
        if (cli->getServer().get_locations()[i].getPath()[0] != '=' && path.find(cli->getServer().get_locations()[i].getPath()) == 0)
        {
            if (cli->getServer().get_locations()[i].getPath().length() > longestMatch.length())
            {
                longestMatch = cli->getServer().get_locations()[i].getPath();
                _exactLocation = i;
            }
        }
    }
    if (!(longestMatch.empty()))
    {
        return;
    }
    return;
}



void WebServ::setExactIndexLocation(int &index) {
    _exactLocation = index;
}

int WebServ::getExactIndexLocation() const {
    return _exactLocation;
}

bool WebServ::is_location_have_redirection(Client* cli)
{
    if (!(cli->getServer().get_locations()[_exactLocation].getReturn().empty()))
    {
        cli->_response.setHeader("Location", cli->getServer().get_locations()[_exactLocation].getReturn());
        cli->_response.setStatus(301);
        cli->_response.setStatusMsg("Moved Permanently");
        cli->_response.setHeader("Content-Length", 0);
        return 1;
    }
    
    return 0;
}

void WebServ::is_method_allowed_in_location(Client* cli)
{
    for (unsigned int i = 0; i < cli->getServer().get_locations()[_exactLocation].getMethods().size(); i++)
    {
        if (cli->getServer().get_locations()[_exactLocation].getMethods()[i] == cli->_request.getMethod())
            return;
    }
    cli->_response.setStatus(405);
    throw(405);
}
