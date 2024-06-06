/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyouub.py <aech-che@127.0.0.1>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 15:28:06 by aech-che          #+#    #+#             */
/*   Updated: 2024/05/31 23:31:44 by ayyouub.py       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/parsing.hpp"


Parsing::Parsing() {
}


Parsing::~Parsing(){
}



int Parsing::parse_file(char *filename, std::vector<std::string> &data)
{

    (void)data;
    std::ifstream infile_count(filename);
    Utils::count_servers(infile_count);
    __NO_SERVERS__ = __NUMBER_OF_SERVERS__;
    std::cout << "We got " << __NUMBER_OF_SERVERS__ << " servers" << std::endl;
    if(!__NO_SERVERS__)
    {
        std::cerr << "No server Detected!" << std::endl;
        return(-1);
    }
    std::ifstream infile(filename);
    std::string buff;
    while (infile)
    {
        std::getline(infile, buff);
        buff = Utils::strtrim(buff);
        // std::cout << "**" << buff << "**" << std::endl;
        if (buff == "router:" || buff == "location:")
            __IN_ROUTER__ = 1;
        else if (buff == "server:")
            __IN_ROUTER__ = 0;

        std::vector<std::string> data_splited  = Utils::split(buff, " ");
    

        

    }
    infile.close();
    return(EXIT_SUCCESS);
}


