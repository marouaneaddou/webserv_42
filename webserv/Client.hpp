#pragma once

# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include "Request.hpp"
# include "Response.hpp"
#include "../includes/servers.hpp"
# include "cgi.hpp"
class Request;
// class Response;
class Client
{
    public:
        Client(int &new_socket);
        ~Client();
        void setCheck();
        void setConf(Servers &server);

        /********************************* CGI ****************************************/

            // bool runCgiLoginRegister(Cgi &cgi);

        /********************************* CGI ****************************************/
        
        bool getCheck() const;
        Response& getResponse();
        Servers getServer();
        // Client(int new_socket, struct sockaddr_in new_addr);
        // std::string		getResponse(void);
        Request _request;
        Response _response;
        int socket;

        /********************** FILE DATA ******************* */

            void setSizeFile(int size);
            int getSizeFile() const;
                        /************************* OPEN , READ DATA***************** */
                        void openFile(std::string name); 
                        /************************* OPEN , READ DATA***************** */
                        std::string getData() const;
                        void setData();
                        
                        bool isOpen() const;

                        void closeFile();
                        
        /********************** FILE DATA ******************* */


    private:
        std::ifstream fileData;
        int sizeFile;
        std::string dataFile;
        bool _check;
        Servers _server;
        struct sockaddr_in _sockaddr;
        std::string raw_request;
        std::string method;

};