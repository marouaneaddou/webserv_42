#pragma once

#include "Client.hpp"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstddef>
#include <cstdlib>
#include <string>
#include <fcntl.h>
#include <dirent.h>

class RequestHandler {
public:
    RequestHandler();
    virtual ~RequestHandler() {}
    virtual void handleRequest(Client* cli) = 0;
protected:
    std::string _path;
    std::string abs_path;
    int _blockIdx;
    // bool is_req_well_formed(const Request* request, Response* response);
    void req_uri_location(Client* cli);
    void is_location_have_redirection(Client* cli);
    void is_method_allowed_in_location(Client* cli);
    void check_requested_method(Client* cli);
    void setStatusMessage(Client* cli);

    ///GET/////
    void get_requested_ressource(Client* cli);
    const std::string get_ressource_type(std::string abs_path);
    bool is_dir_has_index_files(Client* cli);
    bool if_location_has_cgi(Client* cli);
    const size_t getPathSize();
    const std::string getFileContent();
    const std::string getDirListing();
    const std::string getMimeType();

    ////POST///////
    // bool if_location_support_upload(Client* cli);
    // bool ifLocationSupportCgi(Location &location) const;

    //  other checking methods ...

    ////////DELETE///////////
    void handleDeleteRequest(Client* cli, std::string abs_path);
    void deleteDirectoryRecursively(Client* cli, const char* dirPath);
};
