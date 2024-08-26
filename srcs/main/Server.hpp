#ifndef SERVER_HPP
# define SERVER_HPP
#pragma once
# include <vector>
# include <map>
# include <utility>
# include <string>
# include <cstring>
# include <cctype>
# include <cstdlib>
# include <algorithm>
# include <unistd.h>
# include "Location.hpp"
# include "SimpleServer.hpp"

// Represents the configuration for a server
struct Server
{
    typedef std::string t_str;

    SimpleServer* serverptr;

    int port;
    std::string ipAddress;
    std::string permitUploadStr;
    bool        permitUpload;

    std::map<t_str, t_str> statusCodes;

   
    std::string _serverName; //this was vector, i changed to string...
    std::multimap<t_str, t_str> _listen;
    std::map<t_str, Location> _location; 
    std::string _root;
    std::string _index; //string
    std::map<int, t_str> _errorPage;
    unsigned long int _clientMaxBodySize;
    std::vector<t_str> _allowedMethods;
    std::map<std::string,std::string> _cgi; //eg 'sh' : '/bin/bash'
    std::string _return; 
};

Server cleanData(Server server);

#endif
