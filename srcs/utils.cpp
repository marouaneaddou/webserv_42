/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyouub.py <aech-che@127.0.0.1>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 14:39:01 by aech-che          #+#    #+#             */
/*   Updated: 2024/05/31 23:10:49 by ayyouub.py       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/utils.hpp"


Utils::Utils() 
{
    
    


}


Utils::~Utils(){
}

std::string Utils::strtrim(std::string s)
{

	int begin = -1;
	int end = -1;
    std::string trimmed = "";
    

    if(s.length()  == 0)
        return(trimmed);    
    if (s.find(" ") == std::string::npos 
        && s.find("\t") == std::string::npos)
        return(s);
	for (size_t i = 0; i < s.length(); i += 1)
	{
		if ((s[i] != ' ' && s[i] != '\t') && begin != -1)
			end = i;
		else if (s[i] != ' ' && s[i] != '\t')
			begin = i;
		
	}
    trimmed = s.substr(begin, end - 1);
	return(trimmed);
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


