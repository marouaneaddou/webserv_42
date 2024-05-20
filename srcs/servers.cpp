/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   servers.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyouub.py <aech-che@127.0.0.1>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 11:08:23 by ayyouub.py        #+#    #+#             */
/*   Updated: 2024/05/19 12:25:03 by ayyouub.py       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/servers.hpp"


Servers::Servers() 
{
    host = "127.0.0.1";
    server_names.push_back("localhost");
    ports.push_back(80);
    client_body_size = 1000;
    default_server = true;
}


Servers::~Servers(){
}


