#include "Client.hpp"

// Client::Client(int new_socket, struct sockaddr_in new_addr)
// {
//     socket = new_socket;
//     _sockaddr = new_addr;
// }

Client::Client(int &new_socket /*class config*/)
{
    onetime = false;
    socket = new_socket;
    _check = false;
    sizeFile = 0;
    readWriteSize = 0;
    // _request = new Request(/*class config*/);
    // _response = new Response(/*class config*/);
};

Client::~Client(){
    // delete _request;
    // delete _response;
}

void Client::setOnetime()
{onetime = true;}

void Client::setCheck()
{
    _check = true;
}


bool Client::getOnetime() const{
    return onetime;
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
    char buffer[1000000 + 1];
    fileData.read(buffer, 1000000);
    if (fileData.gcount() != 1000000)
        buffer[fileData.gcount()] = '\0';
    readWriteSize += fileData.gcount();
    // std::cout << 
    //std::cout << "read " <<fileData.gcount() << std::endl;

    // _response.setAppendBody(buffer);
    dataFile.append(buffer, fileData.gcount());
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




void Client::setTypeData(int type)
{
    typeData = type;
}

int Client::getTypeData() const 
{
    return typeData;
}


void Client::setreadWriteSize(long long int type)
{
    readWriteSize = type;
}

long long int Client::getreadWriteSize() const 
{
    return readWriteSize;
}

bool Client::checkExtensionFile(std::string path) const {
    std::string extension = path.substr(path.find('.') + 1);
    if (extension != "html" && extension != "js" && extension != "css") return true;
    std::cout << extension << std::endl;
    return false;
} 