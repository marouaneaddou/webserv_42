/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locations.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyouub.py <aech-che@127.0.0.1>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 11:21:18 by ayyouub.py        #+#    #+#             */
/*   Updated: 2024/05/19 12:18:57 by ayyouub.py       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONS_HPP
# define LOCATIONS_HPP

#include "includes.hpp"


class Locations
{
        private:
                std::string path;
                std::string default_file;
                std::vector<std::string> methods;
                std::string directory;
                bool directory_listing;
                std::string cgi_bin;
                std::string cgi_extension;

        public:
            Locations();
            ~Locations();

};