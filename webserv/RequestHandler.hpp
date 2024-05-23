#pragma once
#include "Response.hpp"
#include "Request.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>



class RequestHandler {
public:
    RequestHandler();
    virtual ~RequestHandler() {}
    virtual void handleRequest(const Request* request, Response* response) = 0;
protected:
    std::string _path;
    std::string _pathType;
    bool is_req_well_formed(const Request* request, Response* response);
    bool req_uri_location(const Request* request, Response* response);
    bool is_location_have_redirection(const Request* request, Response* response);
    bool is_method_allowed_in_location(const Request* request, Response* response);
    bool check_requested_method(const Request* request, Response* response);
    bool get_requested_ressource(const Request* request, Response* response);
    void get_ressource_type(const Request* request, Response* response);
    bool is_dir_has_index_files(const Request* request, Response* response);
    bool if_location_has_cgi(const Request* request, Response* response);
    //  other checking methods ...
};
