#include "StaticFileHandler.hpp"
#include "RequestHandler.hpp"

StaticFileHandler::StaticFileHandler()
{}

StaticFileHandler::~StaticFileHandler()
{}

void StaticFileHandler::handleRequest(const Request* request, Response* response)
{
    if (!is_req_well_formed(request, response))
        return;

}