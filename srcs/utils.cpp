/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aech-che <aech-che@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 14:39:01 by aech-che          #+#    #+#             */
/*   Updated: 2024/05/09 15:19:25 by aech-che         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/utils.hpp"


Utils::Utils() {
}


Utils::~Utils(){
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
