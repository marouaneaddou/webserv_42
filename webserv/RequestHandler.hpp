#pragma once
#include "Response.hpp"
#include "Request.hpp"

class RequestHandler {
public:
    RequestHandler();
    virtual ~RequestHandler() {}
    virtual void handleRequest(const Request* request, Response* response) = 0;
protected:
    bool is_req_well_formed(const Request* request, Response* response);
    bool req_uri_location(const Request* request, Response* response);
    //  other checking methods ...
};
