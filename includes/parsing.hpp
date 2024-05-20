/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aech-che <aech-che@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 15:27:43 by aech-che          #+#    #+#             */
/*   Updated: 2024/05/13 15:55:31 by aech-che         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
# define PARSING_HPP

#include "includes.hpp"
#include "utils.hpp"







class Parsing
{
    public:
        Parsing();
        ~Parsing();
        static int parse_file(std::ifstream &infile, std::vector<std::string> &data);




};















#endif
