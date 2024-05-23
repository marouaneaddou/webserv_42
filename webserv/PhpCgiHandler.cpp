#include "PhpCgiHandler.hpp"

PhpCgiHandler::PhpCgiHandler()
{}

PhpCgiHandler::~PhpCgiHandler()
{}

void PhpCgiHandler::handleRequest(const Request* request, Response* response)
{
    if (!is_req_well_formed(request, response))
        return;
    if (!req_uri_location(request, response))
        return;
    if (!is_location_have_redirection(request, response))
        return;
    if (!is_method_allowed_in_location(request, response))
        return;
    if (!check_requested_method(request, response))
        return;
}