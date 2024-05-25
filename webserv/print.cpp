


// # include "webserv.hpp"
# include "Request.hpp"


/*************************** REQUEST **********************************/

void Request::printHeaders() const
{
    for (auto ut : _headers)
    {
        std::cout << ut.first << "============="  <<  ut.second <<std::endl;
    }
}

void Request::printRequest() const
{
    std::cout << _method << " " << _URL << " " << _http_version <<
        "\n" << std::endl ; printHeaders();
    std::cout  /*"\n" << "\n"*/ << std::endl << _body << std::endl;
}

/*************************** REQUEST **********************************/
