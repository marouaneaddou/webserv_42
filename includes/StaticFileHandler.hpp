#pragma once

#include "Client.hpp"
#include "RequestHandler.hpp"

class StaticFileHandler : public RequestHandler {
public:
    StaticFileHandler(int &index, std::string &_path);
    ~StaticFileHandler();
    void handleRequest(Client* cli);
private:
     // Private members for handling static file requests.
};
