#pragma once

#include "Client.hpp"
#include <cstddef>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



class RequestHandler {
public:
    RequestHandler();
    virtual ~RequestHandler() {}
    virtual void handleRequest(Client* cli) = 0;
protected:
    std::string _path;
    // bool is_req_well_formed(const Request* request, Response* response);
    bool req_uri_location(Client* cli);
    bool is_location_have_redirection(Client* cli);
    bool is_method_allowed_in_location(Client* cli);
    bool check_requested_method(Client* cli);
    void setStatusMessage(Client* cli);

    ///GET/////
    bool get_requested_ressource(Client* cli);
    const std::string get_ressource_type(Client* cli);
    bool is_dir_has_index_files(Client* cli);
    bool if_location_has_cgi(Client* cli);
    const size_t getPathSize();

    ////POST///////
    bool if_location_support_upload(Client* cli);


    //  other checking methods ...
};
