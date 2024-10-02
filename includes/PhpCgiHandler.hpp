#pragma once

#include "RequestHandler.hpp"

class PhpCgiHandler : public RequestHandler {
public:
    PhpCgiHandler(int &index, std::string &_path);
    ~PhpCgiHandler();
    void handleRequest(Client* cli);
private:
    // Private members for handling static file requests.
};