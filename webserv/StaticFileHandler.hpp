#pragma once

#include "Response.hpp"
#include "Request.hpp"
#include "RequestHandler.hpp"

class StaticFileHandler : public RequestHandler {
public:
    StaticFileHandler();
    ~StaticFileHandler();
    void handleRequest(const Request* request, Response* response) override;
private:
    // Private members for handling static file requests.
};
