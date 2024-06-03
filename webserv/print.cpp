


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
    printVectOfString();
    // std::cout  /*"\n" << "\n"*/ << std::endl << _body << std::endl;
}

/*************************** REQUEST **********************************/


/*************************** PRINT VECT OF STRING *****************************/

void Request::printVectOfString() const
{
    for (int i = 0; i < _pureBody.size(); i++)
        std::cout << "start "<< _pureBody[i] << " end\n";
}

/*************************** PRINT VECT OF STRING *****************************/
