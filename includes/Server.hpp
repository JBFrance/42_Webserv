#ifndef SERVER_HPP
# define SERVER_HPP

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
# include <unordered_map>
# include "SimpleServer.hpp"

// Represents the configuration for a server
struct Server
{
    typedef std::string t_str;

    // Pointer to the server instance
    SimpleServer* serverptr;

    // Server-specific attributes
    int port;
    std::string ipAddress;
    std::string uploadLocation; // Authorized post origin
    std::string deleteLocation;

    // Status codes and their pages
    std::map<t_str, t_str> statusCodes;

    // Directive lists and server properties
    std::vector<t_str> _directiveList;
    std::vector<t_str> _serverName;
    std::multimap<t_str, t_str> _listen; // Can support multiple listen directives
    std::map<t_str, Location> _location; // Map of locations within the server
    std::string _root;
    std::vector<t_str> _index;
    bool _autoindex;
    std::map<int, t_str> _errorPage;
    unsigned long int _clientMaxBodySize;
    std::vector<t_str> _allowedMethods;
    std::map<t_str, t_str> _cgi;
    std::string _return; // Optional redirect or return status
    std::string _uploadDir;
};



#endif
