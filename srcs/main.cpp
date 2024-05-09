/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aech-che <aech-che@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:35:07 by aech-che          #+#    #+#             */
/*   Updated: 2024/05/09 15:09:55 by aech-che         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/webserv.hpp"


int main(int ac, char **av)
{
        (void)ac;
 
        if(Errors::check_filename(av[1]))
                std::cout << "Error" << std::endl;

}
