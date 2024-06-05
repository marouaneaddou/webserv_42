#pragma once

#include "Client.hpp"
#include "RequestHandler.hpp"

class StaticFileHandler : public RequestHandler {
public:
    StaticFileHandler();
    ~StaticFileHandler();
    void handleRequest(Client* cli);
private:
    // Private members for handling static file requests.
};
