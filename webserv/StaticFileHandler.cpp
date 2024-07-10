#include "StaticFileHandler.hpp"
#include "Client.hpp"

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
            is_location_have_redirection(cli);
            is_method_allowed_in_location(cli);
        }
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