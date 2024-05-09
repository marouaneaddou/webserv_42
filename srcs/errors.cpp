/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aech-che <aech-che@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 11:27:26 by aech-che          #+#    #+#             */
/*   Updated: 2024/05/09 15:27:03 by aech-che         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/errors.hpp"

Errors::Errors() {
}


Errors::~Errors(){
}

size_t stdcount_mine(char *s, char c)
{
    int i = 0;
    int ret = 0;
    while(s[i])
    {
        if(s[i] == c)
            ret += 1;
        i += 1;
    }
    return(ret);
}

int Errors::check_filename(char *file_name)
{
    size_t n = stdcount_mine(file_name, '.');
    if(n != 1)
        return(1);
    std::vector<std::string> vec = Utils::split(file_name, std::string(".", 1));
    if(vec.size() != 2 ||vec[1]  != "conf" )
        return(1);

    return(0);
}