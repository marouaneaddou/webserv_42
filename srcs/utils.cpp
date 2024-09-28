/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maddou <maddou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 14:39:01 by aech-che          #+#    #+#             */
/*   Updated: 2024/09/27 16:04:08 by maddou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/utils.hpp"


Utils::Utils() 
{
    
    


}


Utils::~Utils(){
}
#include <string>
#include <cctype>

std::string Utils::strtrim(std::string s)
{
    size_t start = s.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos)
        return "";

    size_t end = s.find_last_not_of(" \t\n\r\f\v");
    return s.substr(start, end - start + 1);
}




void Utils::count_servers(std::ifstream &infile)
{
    std::string buff;
    while (infile)
    {
        std::getline(infile, buff);
        if (buff == "server:")
            __NUMBER_OF_SERVERS__ += 1;
    }
    infile.close();
}






std::vector<std::string> Utils::split(std::string s, std::string c)
{
    std::vector<std::string> vec;
    size_t pos = 0;
    for(;pos < s.size();)
    {
        pos = s.find(c);
        if ((int)pos == -1)
            break;
        vec.push_back(s.substr(0,pos));
        s.erase(0, pos + 1);
        pos = 0;
    }
    vec.push_back(s);
    return(vec);
}

