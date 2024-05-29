/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servers.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-gand <mel-gand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 11:08:28 by ayyouub.py        #+#    #+#             */
/*   Updated: 2024/05/27 16:01:48 by mel-gand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERS_HPP
# define SERVERS_HPP

#include "includes.hpp"


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
                std::vector<std::map<std::string, std::vector<std::string> > > locations;
        public:
            Servers();
            ~Servers();
            

};


#endif