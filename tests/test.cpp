/*
 * File: test.cpp
 * Created by: 32512
 * Last modified by: 1715612972.0
 * Last modified on: 2024-05-13 16:10:04
 */

#include "../includes/utils.hpp"


Utils::Utils() {
}


Utils::~Utils(){
}



void Utils::count_servers(char *file_name)
{
    std::string buff;
    std::ifstream infile(file_name);
    while ( infile )
    {
        std::getline(infile, buff);
        if (buff == "- server:")
            __NUMBER_OF_SERVERS__ += 1;
    }
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