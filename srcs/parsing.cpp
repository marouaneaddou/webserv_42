/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyouub.py <aech-che@127.0.0.1>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 15:28:06 by aech-che          #+#    #+#             */
/*   Updated: 2024/09/24 13:05:52 by ayyouub.py       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/servers.hpp"
#include "../includes/parsing.hpp"

#include "../includes/utils.hpp"
#include "../includes/errors.hpp"
#include <vector>

Parsing::Parsing() {
}


Parsing::~Parsing(){
}


int Parsing::parse_file(char *filename, std::vector<std::string> &data)
{
    (void)data;
    std::ifstream infile_count(filename);
    if (!infile_count.is_open()) {
        std::cerr << "Error: Could not open file for counting servers!" << std::endl;
        return -1;
    }

    Utils::count_servers(infile_count);
    infile_count.close();  // Ensure to close the file after counting

    __NO_SERVERS__ = __NUMBER_OF_SERVERS__;
    std::cout << "\n[INFO] : We got " << __NUMBER_OF_SERVERS__ << " servers" << std::endl;

    if(!__NO_SERVERS__) {
        std::cerr << "No server detected!" << std::endl;
        return -1;
    }

    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error: Could not open file for parsing!" << std::endl;
        return -1;
    }

    std::string buff;
    int switch_server = 0;
    int line = 0;
    std::string save_arg = "";
    std::vector<Servers> serversvec;
    std::vector<Locations> locationsvec;
    Servers server;
    while (true) {
        
        if(save_arg == "")
        {
            if(!std::getline(infile, buff))
                break;
        }
        else
        {
            buff = save_arg;
            save_arg = "";
        }
        buff = Utils::strtrim(buff);
        if (buff == "router:" || buff == "location:") {
            __IN_ROUTER__ = 1;
        } else if (buff == "server:") {
            __IN_ROUTER__ = 0;
            switch_server += 1;
        }

        if(switch_server % 2 == 0) {
            
            server.set_locations(locationsvec);
            if(!Errors::valid_server_data(server)){
                infile.close();
            }
            serversvec.push_back(server);
            server = Servers();
            switch_server += 1;
            locationsvec.clear();
        }
        else {
            std::string arg;
            std::vector<std::string> data_splited = Utils::split(buff, " ");
            
            arg = data_splited[0];
            if (arg == "host:") {
                if (data_splited.size() == 2)
                    server.set_host(data_splited[1]);
                else {
                    Errors::print_error("Error in host");
                    infile.close();
                    return -1;
                }
            }
            else if (arg == "port:") {
                std::vector<std::string> data_splited_2 = Utils::split(Utils::split(buff, ":")[1], ", ");
                std::vector<int> ports;
                for (std::vector<std::string>::iterator it = data_splited_2.begin(); it != data_splited_2.end(); ++it) {
                    if (!Errors::valid_port(*it)) {
                        Errors::print_error("Error in port");
                        infile.close();
                        return -1;
                    }
                    try {
                        ports.push_back(std::atoi(it->c_str()));
                    } catch (const std::exception&) {
                        Errors::print_error("Error in port, conversion error");
                        infile.close();
                        return -1;
                    }
                }
                server.set_ports(ports);
            }
            else if (arg == "client_body_size:") {
                if (data_splited.size() != 2) {
                    Errors::print_error("Error in client_body_size, too many arguments");
                    infile.close();
                    return -1;
                }
                try {
                    server.set_client_body_size(std::atoi(Utils::strtrim(data_splited[1]).c_str()));
                } catch (const std::exception&) {
                    Errors::print_error("Error in client_body_size, conversion error");
                    infile.close();
                    return -1;
                }
            }
            else if (arg == "server_names:") {
                std::vector<std::string> server_names = Utils::split(Utils::split(buff, ":")[1], ", ");
                for (std::vector<std::string>::iterator it = server_names.begin(); it != server_names.end(); ++it) {
                    if (!Errors::valid_servername(Utils::strtrim(*it))) {
                        Errors::print_error("Error in server names");
                        infile.close();
                        return -1;
                    }
                }
                server.set_server_names(server_names);
            }
            else if (arg == "root:") {
                std::vector<std::string> roots = Utils::split(Utils::split(buff, ":")[1], ", ");
                for (std::vector<std::string>::iterator it = roots.begin(); it != roots.end(); ++it) {
                    if (!Errors::valid_root(Utils::strtrim(*it))) {
                        Errors::print_error("Error in root");
                        infile.close();
                        return -1;
                    }
                }
                server.set_roots(roots);
            }
            else if (arg == "default_server:") {
                if (Utils::strtrim(data_splited[1]) == "true")
                    server.set_default_server(true);
                else
                    server.set_default_server(false);
            }
            else if(arg == "index:")
            {
                std::vector<std::string> index_files = Utils::split(Utils::split(buff, ":")[1], ", ");
                for (std::vector<std::string>::iterator it = index_files.begin(); it != index_files.end(); ++it) {
                    if (!Errors::valid_servername(Utils::strtrim(*it))) {
                        Errors::print_error("Error in index files");
                        infile.close();
                        return -1;
                    }
                }
                server.set_indexFiles(index_files);
            }
            else if (arg == "error_pages") {
                std::vector<std::map<std::string, std::string> > error_pages;
                while (std::getline(infile, buff)) {
                    line += 1;
                    buff = Utils::strtrim(buff);
                    if(buff == "{")
                        continue;
                    if (buff == "}")
                    {
                        save_arg = buff;
                        break;
                    }
                    std::vector<std::string> error_data = Utils::split(buff, " ");
                    if (error_data.size() != 2) {
                        Errors::print_error("Error in error pages, check arguments");
                        infile.close();
                        return -1;
                    }
                    if (!Errors::valid_errorpage(error_data[0])) {
                        Errors::print_error("Error in error pages");
                        infile.close();
                        return -1;
                    }
                    std::map<std::string, std::string> error_page;
                    error_page[error_data[0]] = error_data[1];
                    error_pages.push_back(error_page);
                }
                server.set_error_pages(error_pages);
            }
            else if (arg == "router" || arg == "locations") {
                int return_flag = 0;
                int count = 0;
                Locations location;
                while (std::getline(infile, buff)) {
                    line += 1;
                    buff = Utils::strtrim(buff);
                    if (buff == "}")
                    {
                        locationsvec.push_back(location);
                        save_arg = buff;
                        break;
                    }
                    if(buff == "{")
                        continue;
                    std::vector<std::string> location_data = Utils::split(buff, ": ");
                    arg = Utils::strtrim(location_data[0]);
                    if(arg == "path") {
                        count += 1;
                        if(return_flag > 0) {
                            Errors::print_error("Error in return, only return in location");
                            infile.close();
                            return -1;
                        }
                        location_data = Utils::split(buff, " ");
                        if (location_data.size() != 2){
                            Errors::print_error("Error in path, check arguments");
                            infile.close();
                            return -1;
                        }
                        arg = Utils::strtrim(location_data[1]);
                        if(!Errors::valid_path(arg)) {
                            Errors::print_error("Error in path");
                            infile.close();
                            return -1;
                        }
                        arg.erase(0, 1);
                        arg.erase(arg.size() - 1, 1);
                        location.setPath(arg);
                    }
                    else if(arg == "return") {
                        if(return_flag) {
                            Errors::print_error("Error in return, already set");
                            infile.close();
                            return -1;
                        }
                        if(count > 0) {
                            Errors::print_error("Error in return, only return in location");
                            infile.close();
                            return -1;
                        }
                        return_flag += 1;
                        location_data = Utils::split(buff, " ");
                        if (location_data.size() != 2){
                            Errors::print_error("Error in return, check arguments");
                            infile.close();
                            return -1;
                        }
                        arg = Utils::strtrim(location_data[1]);
                        if(!Errors::valid_return(arg)) {
                            Errors::print_error("Error in return");
                            infile.close();
                            return -1;
                        }
                        arg.erase(0, 1);
                        arg.erase(arg.size() - 1, 1);
                        location.setReturn(arg);
                    }
                    else if(arg == "default_file") {
                        count += 1;
                        if(return_flag > 0) {
                            Errors::print_error("Error in return, only return in location");
                            infile.close();
                            return -1;
                        }
                        if (location_data.size() != 2){
                            Errors::print_error("Error in default file, check arguments");
                            infile.close();
                            return -1;
                        }
                        if(!Errors::valid_defaultfile(location_data[1])) {
                            infile.close();
                            return -1;
                        }
                        location.setDefaultFile(location_data[1]);
                    }
                    else if(arg == "methods"){
                        count += 1;
                        if(return_flag > 0) {
                            Errors::print_error("Error in return, only return in location");
                            infile.close();
                            return -1;
                        }
                        if (location_data.size() != 2){
                            Errors::print_error("Error in methods, check arguments");
                            infile.close();
                            return -1;
                        }
                        std::vector<std::string> methods = Utils::split(location_data[1], ", ");
                        std::vector<std::string> methods_data;
                        for (std::vector<std::string>::iterator it = methods.begin(); it != methods.end(); ++it) {
                            if (!Errors::valid_method(Utils::strtrim(*it))) {
                                Errors::print_error("Error in methods");
                                infile.close();
                                return -1;
                            }
                            methods_data.push_back(Utils::strtrim(*it));
                        }
                        location.setMethods(methods_data);
                    }
                    else if(arg == "index"){
                        count += 1;
                        if(return_flag > 0) {
                            Errors::print_error("Error in return, only return in location");
                            infile.close();
                            return -1;
                        }
                        if (location_data.size() != 2){
                            Errors::print_error("Error in index files, check arguments");
                            infile.close();
                            return -1;
                        }
                        std::vector<std::string> indexfiles = Utils::split(location_data[1], ", ");
                        std::vector<std::string> indexfiles_data;
                        for (std::vector<std::string>::iterator it = indexfiles.begin(); it != indexfiles.end(); ++it) {
                            if (!Errors::valid_indexfiles(Utils::strtrim(*it))) {
                                Errors::print_error("Error in indexfiles");
                                infile.close();
                                return -1;
                            }
                            indexfiles_data.push_back(Utils::strtrim(*it));
                        }
                        location.setIndexFiles(indexfiles_data);
                    }
                    else if(arg == "directory") {
                        count += 1;
                        if(return_flag > 0) {
                            Errors::print_error("Error in return, only return in location");
                            infile.close();
                            return -1;
                        }
                        location_data = Utils::split(buff, " ");
                        if (location_data.size() != 2){
                            Errors::print_error("Error in directory, check arguments");
                            infile.close();
                            return -1;
                        }
                        arg = Utils::strtrim(location_data[1]);
                        if(!Errors::valid_directory(arg)) {
                            Errors::print_error("Error in directory");
                            infile.close();
                            return -1;
                        }
                        arg.erase(0, 1);
                        arg.erase(arg.size() - 1, 1);
                        location.setDirectory(arg);
                    }
                    else if(arg == "directory_listing") {
                        count += 1;
                        if(return_flag > 0) {
                            Errors::print_error("Error in return, only return in location");
                            infile.close();
                            return -1;
                        }
                        location_data = Utils::split(buff, " ");
                        if (location_data.size() != 2){
                            Errors::print_error("Error in directory listing, check arguments");
                            infile.close();
                            return -1;
                        }
                        arg = Utils::strtrim(location_data[1]);
                        if(!Errors::valid_directory_listing(arg)) {
                            Errors::print_error("Error in directory");
                            infile.close();
                            return -1;
                        }
                        if (arg == "on")
                            location.setDirectoryListing(true);
                        else
                            location.setDirectoryListing(false);
                    }
                    else if(arg == "cgi_bin") {
                        count += 1;
                        if(return_flag > 0) {
                            Errors::print_error("Error in return, only return in location");
                            infile.close();
                            return -1;
                        }
                        location_data = Utils::split(buff, " ");
                        if (location_data.size() != 2){
                            Errors::print_error("Error in cgi bin, check arguments");
                            infile.close();
                            return -1;
                        }
                        arg = Utils::strtrim(location_data[1]);
                        if(!Errors::valid_cgi_bin(arg)) {
                            Errors::print_error("Error in cgi bin");
                            infile.close();
                            return -1;
                        }
                        location.setCgiBin(arg);
                    }
                    else if(arg == "root") {
                        count += 1;
                        if(return_flag > 0) {
                            Errors::print_error("Error in return, only return in location");
                            infile.close();
                            return -1;
                        }
                        location_data = Utils::split(buff, " ");
                        if (location_data.size() != 2){
                            Errors::print_error("Error in root location, check arguments");
                            infile.close();
                            return -1;
                        }
                        arg = Utils::strtrim(location_data[1]);
                        if(!Errors::valid_root(arg)) {
                            Errors::print_error("Error in root location");
                            infile.close();
                            return -1;
                        }
                        location.setRoot(arg);
                    }
                    else if(arg == "cgi_extension") {
                        count += 1;
                        if(return_flag > 0) {
                            Errors::print_error("Error in return, only return in location");
                            infile.close();
                            return -1;
                        }
                        location_data = Utils::split(buff, " ");
                        if (location_data.size() != 2){
                            Errors::print_error("Error in cgi extension, check arguments");
                            infile.close();
                            return -1;
                        }
                        arg = Utils::strtrim(location_data[1]);
                        if(!Errors::valid_cgi_extension(arg)) {
                            Errors::print_error("Error in cgi extension");
                            infile.close();
                            return -1;
                        }
                        location.setCgiExtension(arg);
                    }
                    
                    else{
                        if(arg == "{" || arg == "}")
                            continue;
                        Errors::you_mean(arg, 1);
                        infile.close();
                        return -1;
                    }
                    
                }
            }
            else{
                if(arg == "server:" || arg == "{" || arg == "}")
                    continue;
                std::cerr << "[INFO] : Error in line " << line << std::endl;
                Errors::you_mean (Utils::strtrim(arg), 0);
                infile.close();
                return -1;
            }
        }
        line += 1;
    }

    
    
    server.set_locations(locationsvec);
    if(!Errors::valid_server_data(server)){
        infile.close();
    }
    serversvec.push_back(server);

    
    Errors::check_dupservers(serversvec);

    std::vector<std::vector<Servers> > serversvov = parse_server(serversvec);
    
    for (std::size_t i = 0; i < serversvov.size(); ++i)
    {
        std::cout << "Group " << i + 1 << ":" << std::endl;
        
        for (std::size_t j = 0; j < serversvov[i].size(); ++j)
        {
            std::cout << serversvov[i][j].get_host() << std::endl;
        }
        
        std::cout << "-------------------------" << std::endl;
    }



    infile.close();
    return EXIT_SUCCESS;
}




std::vector<std::vector<Servers> > parse_server(std::vector<Servers> serversvec)
{
    std::vector<std::vector<Servers> > result;
    std::vector<bool> visited(serversvec.size(), false);

    for (std::size_t i = 0; i < serversvec.size(); ++i)
    {
        if (visited[i])
            continue;

        std::vector<Servers> group;
        group.push_back(serversvec[i]);
        visited[i] = true;

        for (std::size_t j = i + 1; j < serversvec.size(); ++j)
        {
            if (visited[j])
                continue;

            // Compare servernames, host, and ports
            if (serversvec[i].get_server_names() == serversvec[j].get_server_names() ||
                serversvec[i].get_host() == serversvec[j].get_host() ||
                serversvec[i].get_ports() == serversvec[j].get_ports())
            {
                group.push_back(serversvec[j]);
                visited[j] = true;
            }
        }
        result.push_back(group);
    }

    return result;
}