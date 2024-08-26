#ifndef CGI_HPP
# define CGI_HPP

#include <sys/types.h>
#include <sys/wait.h>
#include "Client.hpp"

class Cgi
{
    private:
        std::map<std::string, std::string> env;
        std::map<std::string, std::string> header;
        Client *cont;
        std::string path;
    public:
        Cgi();
        Cgi(Client *other);
        void initenv();
        void cgi_run();
        Cgi(const Cgi &other);
        Cgi &operator=(Cgi const &other);
        void tofile(std::string path);
        virtual ~Cgi();
        const std::string findscript(std::string uri);
        std::string findquery(std::string uri);

};

#endif