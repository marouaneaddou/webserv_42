
# include "../includes/tester.hpp"
#include "../includes/servers.hpp"
#include "../includes/locations.hpp"


#define COLOR_RESET    "\033[0m"
#define COLOR_RED      "\033[31m"
#define COLOR_GREEN    "\033[32m"
#define COLOR_YELLOW   "\033[33m"
#define COLOR_BLUE     "\033[34m"
#define COLOR_MAGENTA  "\033[35m"
#define COLOR_CYAN     "\033[36m"
#define COLOR_WHITE    "\033[37m"


void __UNIT_TEST__(std::vector<Servers> servers)  {
    
    /******* __TEST__PARSING__ *****/

    std::vector<Servers>::iterator server = servers.begin();
    for( ; server != servers.end(); server++ ) {

    /********* __NUMBER__SERVER__*******/

        std::cout << COLOR_GREEN <<"__serverNumber__ "<< server - servers.begin() << COLOR_GREEN << std::endl;
        /********************************__ERROR__PAGES__**********************************/

            std::vector<std::map<std::string, std::string> >::iterator errorPage = server->get_error_pages().begin();
            std::vector<std::map<std::string, std::string> >::iterator endErrorPage = server->get_error_pages().end();

        for (; errorPage != endErrorPage; ++errorPage) {
            // Now errorPage points to a map, so iterate through the map
            // for (std::map<std::string, std::string>::iterator it = errorPage->begin(); it != errorPage->end(); ++it) {
                std::cout << " __STATUS__CODE : " << errorPage->begin()->first  << " -> " << errorPage->begin()->second << std::endl;
            // }
        }

        /********************************__ERROR__PAGES__**********************************/

        /**************** __GLOBAL__DATA__FOR__SERVER__ ********************/

            std::cout << COLOR_BLUE << std::endl << "                 __GLOBAL_DATA_SERVER__\n" << COLOR_BLUE<< std::endl;

            std::cout << COLOR_WHITE << "                    __host__ : " << COLOR_GREEN<< server->get_host()  << std::endl;
            std::cout << COLOR_WHITE << "                    __bodySize__ : " <<  COLOR_GREEN << server->get_client_body_size() << std::endl;
            std::cout << COLOR_WHITE << "                    __defaultServer__ : " <<  COLOR_GREEN << server->get_default_server() << std::endl<<std::endl;;

            std::cout  << COLOR_BLUE << "                 __GLOBAL_DATA_SERVER__" << std::endl << std::endl;

        /**************** __GLOBAL__DATA__FOR__SERVER__ ********************/

        /************************** __SERVER_PORTS__ *************************/

            std::cout << COLOR_BLUE << std::endl << "                 __SERVER_PORTS__\n" << COLOR_BLUE<< std::endl;

            std::vector<int>::iterator port = server->get_ports().begin();
            std::vector<int>::iterator endPort = server->get_ports().end();

            for (; port != endPort; port++) {
                std::cout << COLOR_WHITE << "                    __PORT_NUMBER__ : " << COLOR_GREEN << *port << std::endl; 
            }
            std::cout << COLOR_BLUE << std::endl << "                 __SERVER_PORTS__\n" << COLOR_BLUE<< std::endl;

        /************************** __SERVER_PORTS__ *************************/

        /************************** __SERVER_NAMES__ *************************/

            std::cout << COLOR_BLUE << std::endl << "                 __SERVER_NAMES__\n" << COLOR_BLUE<< std::endl;
            
            std::vector<std::string>::iterator name = server->get_server_names().begin();
            std::vector<std::string>::iterator endName = server->get_server_names().end();

            for (; name != endName; name++) {
                std::cout << COLOR_WHITE << "                    __NAME__SERVER__ : " << COLOR_GREEN << *name << std::endl; 
            }

            std::cout << COLOR_BLUE << std::endl << "                 __SERVER_NAMES__\n" << COLOR_BLUE<< std::endl;

        /************************** __SERVER_NAMES__ *************************/

        /************************** __SERVER_NAMES__ *************************/

            std::cout << COLOR_BLUE << std::endl << "                 __INDEX__FILES__\n" << COLOR_BLUE<< std::endl;

            std::vector<std::string>::iterator file = server->get_indexFiles().begin();
            std::vector<std::string>::iterator endFile = server->get_indexFiles().end();

            for (; file != endFile; file++) {
                std::cout << COLOR_WHITE << "                    __INDEX__FILE__ : " << COLOR_GREEN << *file << std::endl; 
            }

            std::cout << COLOR_BLUE << std::endl << "                 __INDEX__FILES__\n" << COLOR_BLUE<< std::endl;

        /************************** __SERVER_NAMES__ *************************/

        /************************** __LOCATIONS__ *************************/

        std::cout << COLOR_RED << std::endl << "                                  __LOCATION__\n" << COLOR_BLUE<< std::endl;
        std::vector<Locations>::iterator location = server->get_locations().begin();
        std::vector<Locations>::iterator endLocation = server->get_locations().end();

        for (; location !=  endLocation; location++) {
            std::cout << COLOR_GREEN <<"                                     __NUMBER__LOCATION__ "<< location - server->get_locations().begin() << COLOR_GREEN << std::endl << std::endl;
            std::cout << COLOR_WHITE <<"                                         PATH : " << COLOR_BLUE <<location->getPath() << std::endl;
            std::cout << COLOR_WHITE <<"                                         root : " << COLOR_BLUE <<location->getRoot() << std::endl;
            // std::cout << COLOR_WHITE <<"                                         PATH : " << COLOR_BLUE <<location->getPath() << std::endl;

                    /************************** __INDEX_FILES__ *************************/

            std::cout << COLOR_BLUE << std::endl << "                                         __INDEX__FILES__\n" << COLOR_BLUE<< std::endl;

            std::vector<std::string>::iterator indexFile = location->getIndexFiles().begin();
            std::vector<std::string>::iterator endIndexFile = location->getIndexFiles().end();

            for (; indexFile != endIndexFile; indexFile++) {
                std::cout << COLOR_WHITE << "                                            __INDEX__FILE__ : " << COLOR_GREEN << *indexFile << std::endl; 
            }

            std::cout << COLOR_BLUE << std::endl << "                                         __INDEX__FILES__\n" << COLOR_BLUE<< std::endl;

                    /************************** __INDEX_FILES__ *************************/

            std::cout << COLOR_WHITE <<"                                         CGI : " << COLOR_BLUE <<location->getCgiSupport() << std::endl;
            std::cout << COLOR_WHITE <<"                                         DIRECTORY_LISNING : " << COLOR_BLUE <<location->getDirectoryListing() << std::endl;

                    /************************** __METHODS__ *************************/


            std::cout << COLOR_BLUE << std::endl << "                                         __METHODS__\n" << COLOR_BLUE<< std::endl;

            std::vector<std::string>::iterator method = location->getMethods().begin();
            std::vector<std::string>::iterator endMethod = location->getMethods().end();

            for (; method != endMethod; method++) {
                std::cout << COLOR_WHITE << "                                           __METHOD__ : " << COLOR_GREEN << *method << std::endl; 
            }

            std::cout << COLOR_BLUE << std::endl << "                                         __METHODS__\n" << COLOR_BLUE<< std::endl;

                    /************************** __METHODS__ *************************/

        }
        std::cout << COLOR_RED << std::endl << "                                  __LOCATION__\n" << COLOR_BLUE<< std::endl;
 
        /************************** __LOCATIONS__ *************************/


    }


    /******* __TEST__PARSING__ *****/

}