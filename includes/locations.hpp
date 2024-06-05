/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locations.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-gand <mel-gand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 11:21:18 by ayyouub.py        #+#    #+#             */
/*   Updated: 2024/06/04 17:09:45 by mel-gand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "includes.hpp"
#include <string>
#include <vector>


class Locations
{
        private:
                std::string _path;
                std::string _return;
                std::string default_file;
                std::vector<std::string> methods;
                std::string directory;
                std::string cgi_bin;
                std::string cgi_extension;

        public:
            Locations();
            ~Locations();
            bool directory_listing;
            std::string getPath() const;
            std::string getReturn() const;
            void setReturn(std::string ret);
            void setPath(std::string path);
            std::vector<std::string> getAcceptedMethod() const;

};