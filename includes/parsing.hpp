/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyouub.py <aech-che@127.0.0.1>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 15:27:43 by aech-che          #+#    #+#             */
/*   Updated: 2024/09/24 13:01:20 by ayyouub.py       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_HPP
# define PARSING_HPP

#include "includes.hpp"


// error pages : 400, 401, 403, 404, 405, 500, 501


class Servers;

class Parsing
{
    public:
        Parsing();
        ~Parsing();
        static int parse_file(char *filename, std::vector<std::string> &data);



};




std::vector<std::vector<Servers> > parse_server(std::vector<Servers> serversvec);


void print_server_groups(const std::vector<std::vector<Servers> >& result);







#endif
