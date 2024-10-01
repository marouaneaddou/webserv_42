/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyouub.py <aech-che@127.0.0.1>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 11:08:23 by ayyouub.py        #+#    #+#             */
/*   Updated: 2024/10/01 13:33:00 by ayyouub.py       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/servers.hpp"
#include "../includes/locations.hpp"

Servers::Servers()
{
    client_body_size = 0;
    rootlocation = false;
}


Servers::~Servers(){
}



std::vector<Servers> Servers::get_servers()
{
   return (servers) ;
}

void Servers::set_servers(Servers server)
{
    servers.push_back(server);
}


void Servers::set_host(std::string host)
{
    this->host = host;
}

void Servers::set_ports(std::vector<int> ports)
{
    this->ports = ports;
}

void Servers::set_client_body_size(size_t client_body_size)
{
    this->client_body_size = client_body_size;
}

void Servers::set_server_names(std::vector<std::string> server_names)
{
    this->server_names = server_names;
}

void Servers::set_default_server(bool default_server)
{
    this->default_server = default_server;
}

void Servers::set_locations(std::vector<Locations> locations)
{
    this->locations = locations;
}

void Servers::set_error_pages(std::map<std::string, std::string> error_pages)
{
    this->error_pages = error_pages;
    
}

void Servers::set_root(std::string root)
{
    this->root = root;
}

std::string Servers::get_host()
{
    return (host);
}

std::vector<int> Servers::get_ports()
{
    return (ports);
}

size_t Servers::get_client_body_size()
{
    return (client_body_size);
}

std::vector<std::string> Servers::get_server_names()
{
    return (server_names);
}

bool Servers::get_default_server()
{
    return (default_server);
}

std::vector<Locations> &Servers::get_locations()
{
    return (locations);
}


std::map<std::string, std::string> Servers::get_error_pages()
{
    return (error_pages);
}

std::string Servers::get_root()
{
    return (root);
}

void Servers::set_indexFiles(std::vector<std::string> indexFiles)
{
    this->indexFiles = indexFiles;
}

std::vector<std::string> Servers::get_indexFiles()
{
    return (indexFiles);
}



void Servers::set_rootlocation(bool rootloc){rootlocation = rootloc;}
bool Servers::get_rootlocation(){return (rootlocation);}