#include "PhpCgiHandler.hpp"

PhpCgiHandler::PhpCgiHandler()
{}

PhpCgiHandler::~PhpCgiHandler()
{}

void PhpCgiHandler::handleRequest(const Request* request, Response* response)
{
    if (!is_req_well_formed(request, response))
        return;
}