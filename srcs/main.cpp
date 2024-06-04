/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyouub.py <aech-che@127.0.0.1>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:35:07 by aech-che          #+#    #+#             */
/*   Updated: 2024/05/28 11:16:45 by ayyouub.py       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"



int __NUMBER_OF_SERVERS__ = 0;
int __NO_SERVERS__ = 0;
int __IN_ROUTER__ = 0;



int main(int ac, char **av)
{
        (void)ac;
 
        if(Errors::check_filename(av[1]) || Errors::check_filepermission(av[1]))
                return(EXIT_FAILURE);
        // std::cout << "Reading the conf file...\n";
        std::vector<std::string> data;
        
        if(Parsing::parse_file(av[1], data) == -1)
                return(EXIT_FAILURE);
        
}