/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-gand <mel-gand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 15:27:43 by aech-che          #+#    #+#             */
/*   Updated: 2024/10/06 15:31:31 by mel-gand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
# define PARSING_HPP

#include "includes_util.hpp"



class Servers;

class Parsing
{
    public:
        Parsing();
        ~Parsing();
        static std::map<int, std::vector<Servers> > parse_file(char *filename, std::vector<std::string> &data, std::vector<Servers> &serversvec);



};




std::vector<std::vector<Servers> > parse_server(std::vector<Servers> serversvec);


void print_server_groups(const std::vector<std::vector<Servers> >& result);







#endif
