
#include "WebServer.hpp"
#include <vector>
#include "../includes/servers.hpp"

void fill_Conf(std::vector<Servers> &Confs)
{
    Servers one;
    Servers two;
    Locations first;
    Locations second;

    first.setPath("/");
    // second.setPath("/indexAut.html");
    // first.setReturn("/first");
    // second.setReturn("/second");
    // first.setMethod("GET");
    // first.setMethod("POST");
    first.methods.push_back("GET");
    first.methods.push_back("POST");
    one.roots.push_back("/Users/aech-che/Desktop/webserv_dyal_cgi/webserv/test");
    one.ports.clear();
    one.ports.push_back(2225);
    one.ports.push_back(2223);
    one.ports.push_back(4040);
    one.host = "localhost";
    one.locations.push_back(first);
    one.locations.push_back(second);
    Confs.push_back(one);
    // one.locations.second

    two.ports.clear();
    two.roots.push_back("/Users/aech-che/Desktop/webserv_dyal_cgi/webserv/test");
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