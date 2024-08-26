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

    std::string redirect; // path to redirect
    std::string _location; //added this as will need to access file with _root + _location
    std::string _root;
    std::map<int, t_str> _errorPage;
    unsigned long int _clientMaxBodySize;
    std::vector<t_str> _allowedMethods; 
    std::map<std::string,std::string> _cgi;
    std::string _uploadDir; 
    std::string permitUploadStr;
    bool _permitUpload;
    std::string _return;
};

void printLocationDetails(const Location& location);

#endif