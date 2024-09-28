/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locations.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maddou <maddou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 11:21:18 by ayyouub.py        #+#    #+#             */
/*   Updated: 2024/09/27 16:02:05 by maddou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONS_HPP
# define LOCATIONS_HPP

#include "includes.hpp"
#include <string>
#include <vector>


class Locations
{
        private:
                std::string _path;
                std::string _return;
                std::string default_file;
                std::string directory;
                std::string cgi_bin;
                std::string cgi_extension;
                bool cgi_support;
                std::vector<std::string> _methods;
                bool directory_listing;
                std::vector<std::string> _indexFiles;
                std::string root;
                std::string return_;

        public :      
                Locations();
                ~Locations() {};
                void setPath(std::string path);
                void setReturn(std::string ret);
                void setDefaultFile(std::string file);
                void setDirectory(std::string dir);
                void setCgiBin(std::string bin);
                void setCgiExtension(std::string ext);
                void setCgiSupport(bool support);
                void setMethods(std::vector<std::string> methods);
                void setDirectoryListing(bool listing);
                void setIndexFiles(std::vector<std::string> indexFiles);
                void setRoot(std::string root);
                std::string getRoot();
                std::vector<std::string> getIndexFiles();
                bool getDirectoryListing();
                std::vector<std::string> getMethods();
                std::string getPath();
                std::string getReturn();
                std::string getDefaultFile();
                std::string getDirectory();
                std::string getCgiBin();
                std::string getCgiExtension();
                bool getCgiSupport();
};



#endif