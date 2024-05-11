#include "Tcp_server.hpp"
#include <vector>

int main(int argc, char **argv)
{
    // std::vector <std::vector<int> > ports;
    std::vector<TCPserver> servers;
std::vector<int> port;
port.push_back(4242);
servers.push_back(TCPserver(port));
    // for (int i = 0; i < ports.size(); i++)
    //     servers.push_back(TCPserver(ports[i]));
    while(true)
    {

        for (int i = 0; i < servers.size(); i++)
        {
            servers[i].ready_Rsockets = servers[i].current_Rsockets;
            servers[i].ready_Wsockets = servers[i].current_Wsockets;
            if (select(3, &servers[i].ready_Rsockets, NULL, NULL, NULL) < 0)
                 std::cerr << "Error in select(): " << strerror(errno) << std::endl;
            for (int idx = 0; idx <= 3; idx++)  
            {
                if (FD_ISSET(idx, &servers[i].ready_Rsockets))
                {
                    std::cout << "ok" << std::endl;
                    std::vector<int>::iterator it = std::find(servers[i].getSocket().begin(), servers[i].getSocket().end(), idx);
                    if (it != servers[i].getSocket().end() /*&& servers[i].getClients().size() < MAX_CONNECTIONS*/)
                    {
                        std::cout << idx << std::endl;
                        int new_socket = servers[i].acceptConnection(idx);
                        FD_SET(new_socket, &servers[i].current_Rsockets);
                        FD_SET(new_socket, &servers[i].current_Wsockets);
                    }
                    else
                    {
                        // Handle parsing of the request for the ready for read socket
                        FD_CLR(idx, &servers[i].current_Rsockets);
                    }
                }
                else if (FD_ISSET(idx, &servers[i].ready_Wsockets))
                {
                        // send response to the ready for write socket if the request is fullfilled !
                        FD_CLR(idx, &servers[i].current_Wsockets);
                }
            }


        }
    }


}