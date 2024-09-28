/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maddou <maddou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 14:40:27 by aech-che          #+#    #+#             */
/*   Updated: 2024/09/27 16:04:02 by maddou           ###   ########.fr       */
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
