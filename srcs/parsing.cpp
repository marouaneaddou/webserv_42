/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyouub.py <aech-che@127.0.0.1>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 15:28:06 by aech-che          #+#    #+#             */
/*   Updated: 2024/05/19 11:07:00 by ayyouub.py       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.hpp"


Parsing::Parsing() {
}


Parsing::~Parsing(){
}



int Parsing::parse_file(std::ifstream &infile, std::vector<std::string> &data)
{

    (void)data;
    
    Utils::count_servers(infile);
    __NO_SERVERS__ = __NUMBER_OF_SERVERS__;
    std::cout << "We got " << __NUMBER_OF_SERVERS__ << " servers" <<"\n";
    exit(0);
    if(!__NO_SERVERS__)
    {
        std::cerr << "No server Detected!" << std::endl;
        return(-1);
    }
    std::string buff;
    while ( infile )
    {
        std::getline(infile, buff);
        if (buff == "router:" || buff == "location:")
            __IN_ROUTER__ = 1;
        else if (buff == "server")
            __IN_ROUTER__ = 0;
            
    }
    
    return(EXIT_SUCCESS);
}


