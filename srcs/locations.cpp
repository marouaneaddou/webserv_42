/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locations.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyouub.py <aech-che@127.0.0.1>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 11:22:03 by ayyouub.py        #+#    #+#             */
/*   Updated: 2024/05/19 12:20:01 by ayyouub.py       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/locations.hpp"

Locations::Locations() 
{
    // True -> on
    directory_listing = true;
    cgi_bin = "/usr/bin/bash";
    cgi_extension = ".sh";
}


Locations::~Locations()
{
}
