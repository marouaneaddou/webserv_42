/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyouub.py <aech-che@127.0.0.1>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 11:08:28 by ayyouub.py        #+#    #+#             */
/*   Updated: 2024/10/01 10:55:37 by ayyouub.py       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERS_HPP
# define SERVERS_HPP


#include "includes_util.hpp"
#include "locations.hpp"


class Locations;
class Servers
{
       private:
                std::string host;
                std::vector<int> ports;
                size_t client_body_size;
                std::vector<std::string> server_names;
                std::string root;
                bool default_server;
                std::vector<std::map<std::string, std::string> > error_pages;
                std::vector<std::string> indexFiles;
                // routers or locations
                std::vector<Locations> locations;
                std::vector<std::string> _indexFiles;
                std::vector<Servers> servers;
                bool rootlocation;

        public:
            Servers();
            ~Servers();
            std::vector<Servers> get_servers();
            void set_servers(Servers server);
            void set_host(std::string host);
            void set_ports(std::vector<int> ports);
            void set_client_body_size(size_t client_body_size);
            void set_server_names(std::vector<std::string> server_names);
            void set_root(std::string root);
            void set_default_server(bool default_server);
            void set_error_pages(std::vector<std::map<std::string, std::string> > error_pages);
            void set_locations(std::vector<Locations> locations);
            void set_indexFiles(std::vector<std::string> indexFiles);
            std::string get_host();
            std::vector<int> get_ports();
            size_t get_client_body_size();
            std::vector<std::string> get_server_names();
            std::string get_root();
            bool get_default_server();
            std::vector<std::map<std::string, std::string> > get_error_pages();
            std::vector<Locations> &get_locations();
            std::vector<std::string> get_indexFiles();
            void set_rootlocation(bool rootlocation);
            bool get_rootlocation();
            
        
            
            

};


#endif