#include "../includes/PhpCgiHandler.hpp"
#include "../includes/Client.hpp"

PhpCgiHandler::PhpCgiHandler()
{}

PhpCgiHandler::~PhpCgiHandler()
{}

void PhpCgiHandler::handleRequest(Client* cli)
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
        //cli->_response.generateHeaderResponse();
        //cli->setTypeData(WRITEDATA);

}