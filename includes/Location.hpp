#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <map>
# include <vector>
# include <string>
# include <cstring>
# include <cstdlib>
# include <algorithm>
# include <unistd.h>

# include <iostream>

struct Location
{
    typedef std::string t_str;

    // Directive lists and location-specific attributes
    std::vector<t_str> _directiveList;
    std::string _root;
    std::vector<t_str> _index;
    bool _autoindex;
    bool _isAutoindexed;
    std::map<int, t_str> _errorPage;
    unsigned long int _clientMaxBodySize;
    std::vector<t_str> _allowedMethods;
    std::map<t_str, t_str> _cgi; // CGI directives
    std::string _uploadDir; // Folder to put uploads in
    std::string deleteLocation; // Authorized delete origin
    std::string _return; // Optional redirect or return status
};



#endif