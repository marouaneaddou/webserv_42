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
        std::cout << "test 1337\n";
        check_requested_method(cli);
        std::cout << "test 1338\n";

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