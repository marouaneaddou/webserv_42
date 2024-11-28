/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyouub.py <aech-che@127.0.0.1>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 14:40:27 by aech-che          #+#    #+#             */
/*   Updated: 2024/11/28 09:17:13 by ayyouub.py       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include "includes_util.hpp"
#include <string.h>



extern int __NUMBER_OF_SERVERS__;

extern int __NO_SERVERS__;

extern int __IN_ROUTER__;

class Utils
{

        public:
                Utils();
                ~Utils();
                static std::vector<std::string> split(std::string s, std::string c);
                static void count_servers(std::ifstream &infile);
                static std::string strtrim(std::string s);


};





#endif
