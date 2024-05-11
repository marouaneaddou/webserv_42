#pragma once
#include "Response.hpp"
#include "HTTPRequest.hpp"

class RequestHandler {
public:
    virtual Response handleRequest(const HTTPRequest& request) = 0;
};
