#pragma once

# include <iostream>
# include <sys/socket.h>
# include <netinet/in.h>
# include "Request.hpp"
# include "Response.hpp"
#include "../includes/servers.hpp"
# include "cgi.hpp"

/******** */

# define READDATA 0
# define WRITEDATA 1
# define CLOSESOCKET 2

/******** */

class Request;

class Client
{
    public:
        Client(int &new_socket);
        ~Client();
        void setCheck();
        void setOnetime();
        void setConf(Servers &server);

        /********************************* CGI ****************************************/

            // bool runCgiLoginRegister(Cgi &cgi);

        /********************************* CGI ****************************************/
        
        bool getCheck() const;
        bool getOnetime() const;
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

        void setTypeData(int type);
        int getTypeData() const;

        void setreadWriteSize(long long int size);
        long long int getreadWriteSize() const;
    private:
        bool onetime;
        long long int readWriteSize;
        std::ifstream fileData;

        int sizeFile;

        std::string dataFile;

        bool _check;

        Servers _server;
        struct sockaddr_in _sockaddr;
        std::string raw_request;
        std::string method;


        int typeData;

};