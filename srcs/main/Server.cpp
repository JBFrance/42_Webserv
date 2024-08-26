#include "Server.hpp"

void printServerDetails(const Server& server) 
{
    std::cout << "Server Details:" << std::endl;
    
    std::cout << "  Server Name: " << "[" << server._serverName << "]" << std::endl;
    
    std::cout << "  Listens on:" << std::endl;
    for (const auto& [ip, port] : server._listen) 
    {
        std::cout << "    IP: [" << ip << "]" << std::endl;
        std::cout << "    Port: [" << port << "]" << std::endl;
    }

    std::cout << "  Root: " << "[" << server._root << "]" << std::endl;

    std::cout << "  Index: " << "[" << server._index << "]" << std::endl;

    std::cout << "  Error Pages:" << std::endl;
    for (const auto& [code, page] : server._errorPage) 
    {
        std::cout << "    Code: " << "[" << code << "]" << std::endl;
        std::cout << "    Page: " << "[" << page << "]" << std::endl;
    }

    std::cout << "  Client Max Body Size: " << "[" << server._clientMaxBodySize << "]" << std::endl;

    std::cout << "  Allowed Methods: ";
    for (const auto& method : server._allowedMethods) 
    {
        std::cout << method << " ";
    }
    std::cout << std::endl;

    std::cout << "  CGI:" << std::endl;
    for (const auto& [key, value] : server._cgi) 
    {
        std::cout << "    " << key << " : " << value << std::endl;
    }

    std::cout << "  Return: " << "[" << server._return << "]" << std::endl;

    std::cout << "  Locations:" << std::endl;
    for (const auto& [path, location] : server._location) 
    {
        std::cout << "  Path: " << "[" << path << "]" << std::endl;
        printLocationDetails(location);
    }
}