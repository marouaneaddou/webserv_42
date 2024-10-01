/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maddou <maddou@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 11:27:26 by aech-che          #+#    #+#             */
/*   Updated: 2024/10/01 17:28:54 by maddou           ###   ########.fr       */
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
    data.push_back("{");
    data.push_back("}");
    if(!router_flag)
    {
        data.push_back("server:");
        data.push_back("host:");
        data.push_back("port:");
        data.push_back("client_body_size:");
        data.push_back("server_names:");
        data.push_back("root:");
        data.push_back("default_server:");
        data.push_back("error_pages");
        data.push_back("router");
        data.push_back("location");
        data.push_back("index:");



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
        data.push_back("index:");
        data.push_back("root:");
        data.push_back("return:");

    }
    std::vector<int> values;

    for (size_t i = 0; i < data.size(); i += 1)
    {
        values.push_back(compare(data[i], key));
    }

    if (*max_element(values.begin(), values.end()) > 1)
    {
        std::string value = data[get_pos(values, *max_element(values.begin(), values.end()))];
        value.erase(std::remove(value.begin(), value.end(), ':'), value.end());
        
        std::cerr << "[ERROR] :  Did you mean " <<  value
            << ", instead of " << key << std::endl;
            exit(1);
    }
    else
    {
        std::cerr << "[ERROR] :  Unknown keyword" << std::endl;
        std::cerr << "[TIP]   :  white spaces are also counted and double check {{}}" << std::endl;
        exit(1);
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
        throw("[ERROR] : Too much . in the file name, please try another file <example.conf>\n");
    }
    std::vector<std::string> vec = Utils::split(file_name, std::string(".", 1));
    if(vec.size() != 2 ||vec[1]  != "conf" )
    {
        throw("[ERROR] : Wrong suff <example.conf>\n");
    }

    return(0);
}

int Errors::check_filepermission(char *file_name)
{
    std::ifstream infile(file_name);

    if(!infile.good())
    {
        throw("File do not exist or permissions\n");
    }

    return 0;
}


void Errors::print_error(std::string error)
{
    std::cerr << "[ERROR] : " << error << std::endl;
    exit(EXIT_FAILURE);
}

int Errors::valid_port(std::string port)
{
    port.erase(0, 1);
    if (port.length() < 4 || port.length() > 5)
    {
        throw("Not a valid port, invalid data");
        
    }
    for (size_t i = 0; i < port.length(); i += 1)
    {
        if (!isdigit(port[i]))
        {
            throw("[ERROR] : Not a valid port, invalid data");
            
        }
    }
    // try
    // {
        int port_check = std::stoi(port);
        
        if (port_check < 0 || port_check > 65535)
        {
            throw("[ERROR] : Not a valid port, not in the range");
            
        }
    // // }
    // catch(std::exception& e)
    // {
    //     throw("Error in port, conversion error");
        
    // }
    return (1);
}


int Errors::check_num(std::string input)
{


    for (size_t i = 0; i < input.length(); i += 1)
    {
        if (!isdigit(input[i]))
        {
            throw("[ERROR] : Not a valid client body size, invalid data");
        }
    }
    return(0);
}

int Errors::valid_servername(std::string servername)
{
    if(servername.length() < 3 || servername.length() > 253 
        || servername.find(".") == std::string::npos)
    {
        throw("[ERROR] : Not a valid server name, invalid data");
        
    }
    return(1);
}

int Errors::valid_root(std::string root)
{
    if(root.length() < 3 || root.length() > 253 
        || root.find("/") == std::string::npos)
    {
        throw("[ERROR] : Not a valid root path, invalid data");
        
    }
    return(1);
}

int Errors::valid_defaultserver(std::string ds)
{
    
    if(ds!="on" && ds!="false")
    {
        throw("[ERROR] : Not a valid default server, invalid data");
        
    }
    return(1);
}

// error pages : 400, 401, 403, 404, 405, 500, 501
int Errors::valid_errorpage(std::string ds)
{
    if(ds!="400:" && ds!="401:" && ds!="403:" && ds!="404:" 
    && ds!="405:" && ds!="500:" && ds!="501:" && ds!="502:" 
    && ds!="503:" && ds!="504:" && ds!="505:" && ds!="414:"
    && ds!="409:") 
    {
        throw("Not a valid error page, invalid data");
    }
    return(1);
}


int Errors::exit_error(std::string arg){
    std::vector<std::string> data;
    data.push_back("path:");
    data.push_back("default_file:");
    data.push_back("methods:");
    data.push_back("directory:");
    data.push_back("directory_listing:");
    data.push_back("cgi_bin:");
    data.push_back("cgi_extension:");

    for (size_t i = 0; i < data.size(); i += 1)
    {
        if(arg == data[i])
        {
            return(0);
        } 
    }
    if (arg == "server:" || arg == "location:" || arg == "router:")
    {
        return(0);
    }
    return(1);

}

int Errors::valid_path(std::string arg){
    
    if(arg[0] != '"' || arg[arg.size() - 1] != '"')
    {
        throw("[ERROR] : Error in path, make it in double quotes");
    }
    if(arg.size() < 3)
    {
        throw("[ERROR] : Error in path, invalid data");
    }
    if(arg.find("/") == std::string::npos)
    {
        throw("[ERROR] : Error in path, invalid data");
    }
    return(1);
}

int Errors::valid_defaultfile(std::string arg){
    if(arg.size() < 3)
    {
        throw("[ERROR] : Error in default file, invalid data");
    }
    if(arg.find(".") == std::string::npos)
    {
        throw("[ERROR] : Error in default file, invalid data");
    }
    return(1);
}


int Errors::valid_method(std::string arg){
    if (arg.size() < 3 || arg.size() > 7)
    {
        throw("Error in methods, invalid data");
    }
    if(arg!="GET" && arg!="DELETE" && arg!="POST")
    {
        throw("[ERROR] : Error in methods, invalid data");
    }
    return(1);
}


int Errors::valid_directory(std::string arg){
    
    if(arg[0] != '"' || arg[arg.size() - 1] != '"')
    {
        throw("[ERROR] : Error in directory, make it in double quotes");
    }
    if(arg.size() < 3)
    {
        throw("[ERROR] : Error in directory, invalid data");
    }
    if(arg.find("/") == std::string::npos)
    {
        throw("[ERROR] : Error in directory, invalid data");
    }
    return(1);
}

int Errors::valid_directory_listing(std::string arg){
    
    if(arg != "on" && arg != "off")
    {
        throw("[ERROR] : Error in directory listing, invalid data");
    }
    return(1);
}


int Errors::valid_cgi_bin(std::string arg){
    if(arg != "/usr/local/bin/python3")
    {
        throw("[ERROR] : Error in cgi bin, only python3 allowed {/usr/local/bin/python3}");
    }
    return(1);
}


int Errors::valid_cgi_extension(std::string arg){
    
    if(arg != ".py")
    {
        throw("Error in cgi extension, invalid data");
    }
    return(1);
}


int Errors::valid_indexfiles(std::string indexfile)
{
    if(indexfile.length() < 3 || indexfile.length() > 253 
        || indexfile.find(".") == std::string::npos)
    {
        throw("Not a valid index file, invalid data");
    }
    return(1);
}
int Errors::valid_server_data(Servers &server)
{
    std::cout << std::endl;
    std::cout << "------------------------ UPDATE SERVER ------------------------" << std::endl;
    if(server.get_host().empty()){
        std::cout << "[INFO] : No host found, default [localhost]" << std::endl;
        server.set_host("localhost");
    }
    if(server.get_ports().empty()){
        std::cout << "[INFO] : No port found, default [80]" << std::endl;
        std::vector<int> ports;
        ports.push_back(80);
        server.set_ports(ports);
    }
    if(server.get_client_body_size() == 0){
        std::cout << "[INFO] : No client body size found, default [1M]" << std::endl;
        server.set_client_body_size(1000000);
    }
    if(server.get_server_names().empty()){
        std::cout << "[INFO] : No server names found, default [localhost]" << std::endl;
        std::vector<std::string> server_names;
        server_names.push_back("localhost");
        server.set_server_names(server_names);
    }

    if(server.get_root().empty()){
        throw("[ERROR] : No root found");
    }
    if(server.get_indexFiles().empty()){
        throw("[ERROR] : No index files found");
    }
    if(server.get_default_server() == 0){
        std::cout << "[INFO] : No default server found, default [false]" << std::endl;
        server.set_default_server(false);
    }
    if(server.get_error_pages().empty()){
        std::cout << "[INFO] : No error pages found, default [400, 401, 403, 404, 405, 500, 501]" << std::endl;
    }

    // std::vector<Locations> &locations = server.get_locations();  // Use a reference to modify original data
    for (std::vector<Locations>::iterator it = server.get_locations().begin(); it != server.get_locations().end(); ++it)
    {
        if(it->getReturn() != ""){
            if(it->getPath().empty())
               throw("[ERROR] : No path found");
            else
                continue;
        }
        if(it->getPath().empty()){
            throw("[ERROR] : No path found");
        }
        if(it->getMethods().empty()){
            throw("[ERROR] : No methods found");
        }
        if(it->getRoot().empty()){
            std::cout << "[INFO] : No location root found, default [server root]" << std::endl;
            it->setRoot(server.get_root());  // Modify the original location
        }
        if(it->getIndexFiles().empty()){
            std::cout << "[INFO] : No location index file, default [server index]" << std::endl;
            it->setIndexFiles(server.get_indexFiles());
        }
        if(it->getDirectoryListing() == 0){
            std::cout << "[INFO] : Direcotry listing set to false" << std::endl;
            it->setDirectoryListing(false);
        }
        if(it->getCgiExtension().empty()){
            std::cout << "[INFO] : No cgi extension found, default [.py]" << std::endl;
            it->setCgiExtension(".py");
        }
        if(it->getCgiBin().empty()){
            std::cout << "[INFO] : No cgi extension found, default [/usr/local/bin/python3]" << std::endl;
            it->setCgiBin("/usr/local/bin/python3");
        }
    }
    std::cout << "------------------------ SERVER UPDATED ------------------------" << std::endl;
    std::cout << std::endl;


    return(1);
    
}

bool compare_vector_of_strings(const std::vector<std::string>& v1, const std::vector<std::string>& v2) {
    for (size_t i = 0; i < v1.size(); ++i) {
        for (size_t j = 0; j < v2.size(); ++j) {
            if (v1[i] == v2[j]) {
                return true;
            }
        }
    }
    return false;
}


std::string join_server_names(const std::vector<std::string>& names) {
    std::ostringstream oss;
    for (size_t i = 0; i < names.size(); ++i) {
        oss << names[i];
        if (i != names.size() - 1) {
            oss << ", ";
        }
    }
    return oss.str();
}

bool compare_servers(Servers& s1, Servers& s2) {
    if (compare_vector_of_strings(s1.get_server_names(), s2.get_server_names())) {
        if (s1.get_host() == s2.get_host()) {
            return true;
        }
    }
    return false;
}

void Errors::check_dupservers(std::vector<Servers> serversvec)
{
    for (std::vector<Servers>::iterator it1 = serversvec.begin(); it1 != serversvec.end(); ++it1)
    {
        for (std::vector<Servers>::iterator it2 = it1 + 1; it2 != serversvec.end(); ++it2)
        {
            if (compare_servers(*it1, *it2))
            {
                Errors::print_error("Duplicate server found with the same name and host: "
                          + join_server_names(it1->get_server_names()) + ", " 
                          + it1->get_host());
            }
        }
    }
}

int Errors::valid_return(std::string arg){
    
    if(arg[0] != '"' || arg[arg.size() - 1] != '"')
    {
        Errors::print_error("Error in return, make it in double quotes");
        return(0);
    }
    if(arg.size() < 3)
    {
        Errors::print_error("Error in return, invalid data");
        return(0);
    }
    if(arg.find("/") == std::string::npos)
    {
        Errors::print_error("Error in return, invalid data");
        return(0);
    }
    return(1);
}



