/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-gand <mel-gand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 14:40:27 by aech-che          #+#    #+#             */
/*   Updated: 2024/09/30 00:48:20 by mel-gand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include "includes_util.hpp"




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
