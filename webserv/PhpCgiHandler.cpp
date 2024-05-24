#include "PhpCgiHandler.hpp"
#include "Client.hpp"

PhpCgiHandler::PhpCgiHandler()
{}

PhpCgiHandler::~PhpCgiHandler()
{}

void PhpCgiHandler::handleRequest(Client* cli)
{
    // if (!is_req_well_formed(request, response))
    //     return;
    if (!req_uri_location(cli))
        return;
    if (!is_location_have_redirection(cli))
        return;
    if (!is_method_allowed_in_location(cli))
        return;
    if (!check_requested_method(cli))
        return;
}