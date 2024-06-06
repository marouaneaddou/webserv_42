


# ifndef CGI_HPP
# define CGI_HPP

# include "Request.hpp"

class Cgi
{
    private:
        Request _request;
    public:
        Cgi(Request &request);
        ~Cgi();
};

# endif