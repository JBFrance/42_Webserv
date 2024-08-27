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
#include <sstream>
# include "Location.hpp"
# include "SimpleServer.hpp"

// Represents the configuration for a server
struct Server
{
    typedef std::string t_str;

    SimpleServer* serverptr;

    int port;
    std::string         ipAddress;
    std::string         permitUploadStr;
    bool                permitUpload;

    std::map<t_str, t_str> statusCodes;

   
    std::string _serverName;
    std::multimap<t_str, t_str> _listen;
    std::vector<Location> _location; //changed from map to vector
    std::string _root;
    std::string _index; //string
    std::map<int, t_str> _errorPage;
    unsigned long int _clientMaxBodySize;
    std::vector<t_str> _allowedMethods;
    std::map<std::string,std::string> _cgi; //eg 'sh' : '/bin/bash'
    std::string _return; 
};

void parseConfigFile(const std::string& filename, std::vector<Server>& servers);
void parseServerBlock(std::istringstream& stream, Server& server);
void parseLocationBlock(std::istringstream& stream, Location& location);
unsigned long parseClientBodyMaxSize(const std::string& line);
unsigned long safeStoul(const std::string& str);
void printServerDetails(const Server& server);

Server cleanData(Server server);

#endif
