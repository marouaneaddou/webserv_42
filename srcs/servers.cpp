/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maddou <maddou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 11:08:23 by ayyouub.py        #+#    #+#             */
/*   Updated: 2024/09/30 12:08:11 by maddou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/servers.hpp"
#include "../includes/locations.hpp"

Servers::Servers()
{
    client_body_size = 0;
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

void Servers::set_error_pages(std::vector<std::map<std::string, std::string> > error_pages)
{
    this->error_pages = error_pages;
    
}

void Servers::set_roots(std::vector<std::string> roots)
{
    this->roots = roots;
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


std::vector<std::map<std::string, std::string> > Servers::get_error_pages()
{
    return (error_pages);
}

std::vector<std::string> Servers::get_roots()
{
    return (roots);
}

void Servers::set_indexFiles(std::vector<std::string> indexFiles)
{
    this->indexFiles = indexFiles;
}

std::vector<std::string> Servers::get_indexFiles()
{
    return (indexFiles);
}



