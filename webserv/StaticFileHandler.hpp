#pragma once

#include "Response.hpp"
#include "HTTPRequest.hpp"
#include "RequestHandler.hpp"

class StaticFileHandler : public RequestHandler {
public:
    Response handleRequest(const HTTPRequest& request) override;
private:
    // Private members for handling static file requests.
};
