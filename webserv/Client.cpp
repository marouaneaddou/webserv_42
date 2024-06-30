#include "Client.hpp"

// Client::Client(int new_socket, struct sockaddr_in new_addr)
// {
//     socket = new_socket;
//     _sockaddr = new_addr;
// }

Client::Client(int &new_socket /*class config*/)
{
    socket = new_socket;
    _check = false;
    sizeFile = -1;
    // _request = new Request(/*class config*/);
    // _response = new Response(/*class config*/);
};

Client::~Client(){
    // delete _request;
    // delete _response;
}

void Client::setCheck()
{
    _check = true;
}

bool Client::getCheck() const
{
    return _check;
}

Response &Client::getResponse()
{
    return _response;
}

void Client::setConf(Servers &server)
{
    _server = server;
}

Servers Client::getServer()
{
    return _server;
}





/********************************* CGI ****************************************/

    // bool Client::runCgiLoginRegister(Cgi &cgi)
    // {
        
    // }

/********************************* CGI ****************************************/



/*************************** DATA FILE **************************** */

void Client::setSizeFile(int size) 
{
    sizeFile = size;
}

int Client::getSizeFile() const
{
    return sizeFile;
}


void Client::openFile(std::string name)
{
    fileData.open(name);
    if (!fileData.is_open())
    {
        perror("Error: open file");
        exit(1);
    }
    fileData.seekg (0, fileData.end);
    sizeFile = fileData.tellg();
    fileData.seekg (0, fileData.beg);
}

void Client::setData()
{
    if (fileData.is_open())
    {
        char buffer[1000000 + 1];
        fileData.read(buffer, 1000000);
        // std::cout << buffer << std::endl;
        if (fileData.gcount() != 1000000)
            buffer[fileData.gcount()] = '\0';
        std::cout << "hnya\n"; 
        std::cout << fileData.gcount() << std::endl;
        sizeFile -= fileData.gcount();
        std::cout << sizeFile << std::endl;
        dataFile.append(buffer, fileData.gcount());// += buffer;
        std::cout << "hnya\n"; 
    }
    else 
        std::cout << "ERROR IN OPEN FILE" << std::endl;
}

std::string Client::getData() const
{
    return dataFile;
}

bool Client::isOpen() const 
{
    return fileData.is_open();
}

void Client::closeFile()
{
    fileData.close();
}
/*************************** DATA FILE **************************** */
