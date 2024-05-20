/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aech-che <aech-che@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 11:27:44 by aech-che          #+#    #+#             */
/*   Updated: 2024/05/10 15:47:12 by aech-che         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_HPP
# define ERRORS_HPP

#include "includes.hpp"
#include "utils.hpp"


class Errors
{
    public:
        Errors();
        ~Errors();
        static int check_filename(char *file_name);
        static int check_filepermission(char *file_name);
        static int valid_keyword(std::string key, int router_flag);
        static void you_mean(std::string key, int router_flag);

};















#endif
