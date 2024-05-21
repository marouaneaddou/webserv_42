#pragma once

#include "Response.hpp"
#include "Request.hpp"
#include "RequestHandler.hpp"

class PhpCgiHandler : public RequestHandler {
public:
    PhpCgiHandler();
    ~PhpCgiHandler();
    void handleRequest(const Request* request, Response* response);
private:
    // Private members for handling static file requests.
};