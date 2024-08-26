#include "Server.hpp"
#include "Location.hpp"

void printLocationDetails(const Location& location) 
{
    std::cout << "Location Details:" << std::endl;
    std::cout << "  Redirect: " << location.redirect << std::endl;
    std::cout << "  Location Path: " << location._location << std::endl;
    std::cout << "  Root: " << location._root << std::endl;
    
    std::cout << "  Error Pages:" << std::endl;
    for (const auto& [code, page] : location._errorPage) 
        std::cout << "    Code: " << code << ", Page: " << page << std::endl;

    std::cout << "  Client Max Body Size: " << location._clientMaxBodySize << std::endl;

    std::cout << "  Allowed Methods: ";
    for (const auto& method : location._allowedMethods) 
        std::cout << method << " ";
    std::cout << std::endl;

    std::cout << "  CGI:" << std::endl;
    for (const auto& [key, value] : location._cgi) 
        std::cout << "    " << key << " : " << value << std::endl;

    std::cout << "  Upload Directory: " << location._uploadDir << std::endl;
    std::cout << "  Permit Upload String: " << location.permitUploadStr << std::endl;
    std::cout << "  Permit Upload: " << (location._permitUpload ? "Yes" : "No") << std::endl;
    std::cout << "  Return: " << location._return << std::endl;
}