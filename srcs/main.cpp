/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maddou <maddou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 11:35:07 by aech-che          #+#    #+#             */
/*   Updated: 2024/10/01 14:53:35 by maddou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/includes.hpp"




int __NUMBER_OF_SERVERS__ = 0;
int __NO_SERVERS__ = 0;
int __IN_ROUTER__ = 0;



int main(int ac, char **av)
{
        (void)ac;
 
        // std::cout << "Reading the conf file...\n";
        try {
                Errors::check_filename(av[1]);
                Errors::check_filepermission(av[1]);
                std::vector<std::string> data;
                std::ifstream infile(av[1]);
                std::vector<Servers> serverConf;
                std::map<int, std::vector<Servers> > configFile;
                configFile = Parsing::parse_file(av[1], data, serverConf);
                // for (size_t i = 0; i < configFile.size(); i++) {
                        // __UNIT_TEST__(serverConf);
                // }
                WebServ WebServ;
                WebServ.run_servers(configFile, serverConf);
                
        }
        catch(char const* e) {
                std::cout << e << std::endl;
        }
        catch (const std::invalid_argument& e) {
                std::cerr << "Invalid argument: could not convert string to an integer." << std::endl;
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Out of range: the number is too large to fit in an int." << std::endl;
        }
        catch (std::string &err) {
                std::cout << err <<std::endl;
        }
        
}