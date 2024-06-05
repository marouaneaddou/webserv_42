#pragma once

#include "Client.hpp"
#include "RequestHandler.hpp"

class PhpCgiHandler : public RequestHandler {
public:
    PhpCgiHandler();
    ~PhpCgiHandler();
    void handleRequest(Client* cli);
private:
    // Private members for handling static file requests.
};