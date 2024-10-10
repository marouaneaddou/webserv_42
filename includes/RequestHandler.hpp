#pragma once

#include "Client.hpp"
#include "locations.hpp"
#include "includes_util.hpp"

class RequestHandler {
public:
    RequestHandler(){};
    RequestHandler(int &index, std::string &_path);
     ~RequestHandler() {
    }
     void handleRequest(Client* cli);
protected:
    std::string _path;
    std::string abs_path;
    int _blockIdx;
    void req_uri_location(Client* cli);
    void is_location_have_redirection(Client* cli);
    void is_method_allowed_in_location(Client* cli);
    void check_requested_method(Client* cli);
    void setStatusMessage(Client* cli);
    void get_requested_ressource(Client* cli);
    const std::string get_ressource_type(std::string abs_path);
    bool is_dir_has_index_files(Client* cli);
    bool if_location_has_cgi(Client* cli);
    size_t getPathSize();
    const std::string getFileContent();
    const std::string getDirListing();
    const std::string getMimeType();
    void handleDeleteRequest(Client* cli, std::string abs_path);
    void deleteDirectoryRecursively(Client* cli, const char* dirPath);
};
