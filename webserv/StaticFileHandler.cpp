#include "StaticFileHandler.hpp"
#include "Client.hpp"

StaticFileHandler::StaticFileHandler()
{}

StaticFileHandler::~StaticFileHandler()
{}

void StaticFileHandler::handleRequest(Client* cli)
{
    // if (!is_req_well_formed(request, response))
    //     return;
    if (req_uri_location(cli) == EXIT_FAILURE)
        return;
    if (is_location_have_redirection(cli) == EXIT_FAILURE)
        return;
    if (is_method_allowed_in_location(cli) == EXIT_FAILURE)
        return;
    if (check_requested_method(cli) == EXIT_FAILURE)
        return;
    setStatusMessage(cli);
}