/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-gand <mel-gand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 15:27:43 by aech-che          #+#    #+#             */
/*   Updated: 2024/06/01 06:48:51 by mel-gand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
# define PARSING_HPP

#include "utils.hpp"







class Parsing
{
    public:
        Parsing();
        ~Parsing();
        static int parse_file(char *filename, std::vector<std::string> &data);




};















#endif
