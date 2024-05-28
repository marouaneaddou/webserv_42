#include "Tcp_server.hpp"
#include "Webserver.hpp"
#include <vector>
#include "../includes/servers.hpp"

void fill_Conf(std::vector<Servers> &Confs)
{
    Servers one;
    Servers two;
    one.ports.clear();
    one.ports.push_back(2210);
    one.ports.push_back(1452);
    one.ports.push_back(4040);
    one.host = "127.0.0.0";
    Confs.push_back(one);

    two.ports.clear();
    two.ports.push_back(1030);
    two.ports.push_back(1052);
    two.ports.push_back(4242);
    two.host = "192.07.20.30";
    Confs.push_back(two);
}

int main(int argc, char **argv)
{
    WebServ WebServ;
    std::vector<Servers> Confs;
    fill_Conf(Confs);
    WebServ.run_servers(Confs);
}