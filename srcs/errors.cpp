/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ayyouub.py <aech-che@127.0.0.1>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 11:27:26 by aech-che          #+#    #+#             */
/*   Updated: 2024/05/19 11:04:49 by ayyouub.py       ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/errors.hpp"

Errors::Errors() {
}


Errors::~Errors(){
}


int Errors::valid_keyword(std::string key, int router_flag)
{
    (void)key;
    std::vector<std::string> keywords;
    std::vector<std::string> router_keywords;
    if(!router_flag)
    {
        keywords.push_back("host:");
        keywords.push_back("port:");
        keywords.push_back("client_body_size:");
        keywords.push_back("server_names:");
        keywords.push_back("root:");
        keywords.push_back("default_server:");
        keywords.push_back("error_pages:");
        keywords.push_back("router:");
        keywords.push_back("location:");
        for (size_t i = 0; i < keywords.size(); i += 1)
        {
            if(key == keywords[(int)i])
                return(1);
        }
    }
    else
    {
        // router location
        router_keywords.push_back("path:");
        router_keywords.push_back("default_file:");
        router_keywords.push_back("methods:");
        router_keywords.push_back("directory:");
        router_keywords.push_back("directory_listing:");
        router_keywords.push_back("cgi_bin:");
        router_keywords.push_back("cgi_extension:");
        for (size_t i = 0; i < router_keywords.size(); i += 1)
        {
            if(key == router_keywords[(int)i])
                return(1);
        }
    }
    
    
    return (0);
    
}

int compare(std::string s1, std::string s2)
{
    size_t len_s1 = s1.size();
    size_t len_s2 = s2.size();
    int i = 0;
    int count = 0;
    while (len_s1 && len_s2)
    {
        if(s1[i] == s2[i])
            count += 1;

        i += 1;
        len_s1 -= 1;
        len_s2 -= 1;
    }
    return (count);
}


int get_pos(std::vector<int> data, int value)
{
    for (size_t i = 0; i < data.size(); i += 1)
    {
        if (data[i] == value)
            return(i);
    } 
    return(0);
}

void Errors::you_mean(std::string key, int router_flag)   
{
    std::vector<std::string> data;
    if(!router_flag)
    {
        data.push_back("host:");
        data.push_back("port:");
        data.push_back("client_body_size:");
        data.push_back("server_names:");
        data.push_back("root:");
        data.push_back("default_server:");
        data.push_back("error_pages:");
        data.push_back("router:");
        data.push_back("location:");
    }
    else
    {
        // router location
        data.push_back("path:");
        data.push_back("default_file:");
        data.push_back("methods:");
        data.push_back("directory:");
        data.push_back("directory_listing:");
        data.push_back("cgi_bin:");
        data.push_back("cgi_extension:");
    }
    std::vector<int> values;

    for (size_t i = 0; i < data.size(); i += 1)
    {
        values.push_back(compare(data[i], key));
    }
    if (*max_element(values.begin(), values.end()) > 1)
    {
        std::cerr << "Did you mean " << data[get_pos(values, *max_element(values.begin(), values.end()))] 
            << ", instead of " << key << std::endl;
    }
    else
    {
        std::cerr << "Unknown keyword" << std::endl;
    }
}

size_t stdcount_mine(char *s, char c)
{
    int i = 0;
    int ret = 0;
    while(s[i])
    {
        if(s[i] == c)
            ret += 1;
        i += 1;
    }
    return(ret);
}

int Errors::check_filename(char *file_name)
{
    size_t n = stdcount_mine(file_name, '.');
    if(n != 1)
    {
        std::cerr << "Too much . in the file name, please try another file <example.conf>\n";
        return(EXIT_FAILURE);
    }
    std::vector<std::string> vec = Utils::split(file_name, std::string(".", 1));
    if(vec.size() != 2 ||vec[1]  != "conf" )
    {
        std::cerr << "Wrong suff <example.conf>\n";
        return(EXIT_FAILURE);
    }

    return(0);
}

int Errors::check_filepermission(char *file_name)
{
    std::ifstream infile(file_name);

    if(!infile.good())
    {
        std::cerr << "File do not exist or permissions\n";
        return(EXIT_FAILURE);
    }

    return 0;
}
