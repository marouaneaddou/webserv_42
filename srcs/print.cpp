


#include "../includes/request.hpp"

/*************************** REQUEST **********************************/

void Request::printHeaders() const
{
    // for (auto ut : _headers)
    // {
    //     std::cout << ut.first << "============="  <<  ut.second <<std::endl;
    // }
}

void Request::printRequest() const
{
    std::cout << _method << " " << _URL << " " << _http_version <<
        "\n" << std::endl ; 
        printHeaders();
    std::cout << std::endl << _body << std::endl;
}

/*************************** REQUEST **********************************/


/*************************** PRINT VECT OF STRING *****************************/

/*THIS NOT WORK IN LAST CODE YOU MUST REMOVE MAP `_PUREBODY` ~!!!!!!!!!!~*/

void Request::printVectOfString() const
{
    // std::cout << "size of pure body ===> " << _pureBody.size() << std::endl;
    // for (int i = 0; i < _pureBody.size(); i++)
    //     // std::cout << "start "<< _pureBody[i] << " end\n";
}

/*************************** PRINT VECT OF STRING *****************************/
