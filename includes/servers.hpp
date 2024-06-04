/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-gand <mel-gand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 11:08:28 by ayyouub.py        #+#    #+#             */
/*   Updated: 2024/06/04 16:50:03 by mel-gand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERS_HPP
# define SERVERS_HPP


#include "includes.hpp"
#include "locations.hpp"
#include <vector>

class Servers
{
       public:
                std::string host;
                std::vector<int> ports;
                size_t client_body_size;
                std::vector<std::string> server_names;
                std::vector<std::string> roots;
                bool default_server;
                std::vector<std::map<std::string, std::string> > error_pages;
                // routers or locations
                std::vector<Locations> locations;
                std::vector<std::string> _indexFiles;

        public:
            Servers();
            ~Servers();
            

};


#endif