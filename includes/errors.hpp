/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aech-che <aech-che@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 11:27:44 by aech-che          #+#    #+#             */
/*   Updated: 2024/05/09 15:06:15 by aech-che         ###   ########.fr       */
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
        
};















#endif