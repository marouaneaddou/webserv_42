/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-gand <mel-gand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 11:27:44 by aech-che          #+#    #+#             */
/*   Updated: 2024/09/30 00:45:18 by mel-gand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_HPP
# define ERRORS_HPP

#include "includes_util.hpp"
#include "servers.hpp"
#include "locations.hpp"
#include "utils.hpp"


class Servers;
class Locations;
class Errors
{
    public:
        Errors();
        ~Errors();
        static int check_filename(char *file_name);
        static int check_filepermission(char *file_name);
        static int valid_keyword(std::string key, int router_flag);
        static void you_mean(std::string key, int router_flag);
        static void print_error(std::string error);
        static int valid_port(std::string port);
        static int check_num(std::string input);
        static int valid_servername(std::string servername);
        static int valid_root(std::string root);
        static int valid_defaultserver(std::string ds);
        static int valid_errorpage(std::string ds);
        static int exit_error(std::string arg);
        static int valid_path(std::string arg);
        static int valid_defaultfile(std::string arg);
        static int valid_method(std::string arg);
        static int valid_directory(std::string arg);
        static int valid_directory_listing(std::string arg);
        static int valid_cgi_bin(std::string arg);
        static int valid_cgi_extension(std::string arg);
        static int valid_indexfiles(std::string indexfile);
        static int valid_server_data(Servers &server);
        static void check_dupservers(std::vector<Servers> serversvec);
        static int valid_return(std::string arg);
};















#endif
