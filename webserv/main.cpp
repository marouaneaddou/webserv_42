#include "Tcp_server.hpp"
#include "Webserver.hpp"
#include <vector>
#include "../includes/servers.hpp"

void fill_Conf(std::vector<Servers> &Confs)
{
    Servers one;
    Servers two;
    one.roots.push_back("/Users/mel-gand/Desktop/webserv_git/webserv/test");
    one.ports.clear();
    one.ports.push_back(2225);
    one.ports.push_back(2223);
    one.ports.push_back(4040);
    one.host = "localhost";
    Confs.push_back(one);

    two.ports.clear();
    two.roots.push_back("/Users/mel-gand/Desktop/webserv_git/webserv/");
    two.ports.push_back(7777);
    two.ports.push_back(3052);
    two.ports.push_back(4242);
    two.host = "localhost";
    Confs.push_back(two);
}

int main(int argc, char **argv)
{
    WebServ WebServ;
    std::vector<Servers> Confs;
    fill_Conf(Confs);
    WebServ.run_servers(Confs);
}