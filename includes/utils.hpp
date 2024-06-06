/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aech-che <aech-che@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 14:40:27 by aech-che          #+#    #+#             */
/*   Updated: 2024/05/13 15:52:32 by aech-che         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_HPP
# define UTILS_HPP

#include "includes.hpp"



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
