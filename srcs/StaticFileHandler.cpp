#include "../includes/StaticFileHandler.hpp"
#include "../includes/Client.hpp"

StaticFileHandler::StaticFileHandler()
{}

StaticFileHandler::~StaticFileHandler()
{}

void StaticFileHandler::handleRequest(Client* cli)
{
    try
    {
        if (cli->getOnetime() == false)
        {
            req_uri_location(cli);
            std::cout << "Location: " << cli->getServer().get_locations()[_blockIdx].getPath() << std::endl;
            std::cout << "Location BlockIdx: " << _blockIdx << std::endl;
            std::cout << "Location Root: " << cli->getServer().get_locations()[_blockIdx].getRoot() << std::endl;
            std::cout << "Method: " << cli->_request.getMethod() << std::endl;
            std::cout << "Path: " << _path << std::endl;
            if (_blockIdx != -1) {
                is_location_have_redirection(cli); 
                is_method_allowed_in_location(cli);
            }
        }
        setStatusMessage(cli);
        check_requested_method(cli);

    }
    catch (...)
    {
    }
    setStatusMessage(cli);
    if (cli->_response.getStatus() != 200)
    {
        cli->_response.generateHeaderResponse();
        cli->setTypeData(WRITEDATA);
    }
}