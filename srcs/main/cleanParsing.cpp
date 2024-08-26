#include "Server.hpp"
#include "Location.hpp"

std::string trimm(const std::string& str) 
{
    size_t start = 0;
    size_t end = str.size();

    // Find the first non-space character
    while (start < end && std::isspace(str[start]))
        ++start;
    while (end > start && std::isspace(str[end - 1])) 
        --end;
    return (start < end) ? str.substr(start, end - start) : "";
}

std::string extractString(const std::string& data) 
{
    size_t pos = data.find_first_not_of(" \t");

    if (pos == std::string::npos) 
        return "";

    size_t endPos = data.find_first_of(" \t", pos);

    if (endPos == std::string::npos)
        return trimm(data.substr(pos));

    return trimm(data.substr(pos, endPos - pos));
}

Server cleanData(Server server)
{
    server._serverName = extractString(server._serverName);
    std::cout << "1" << server._serverName << std::endl;
    return server;
}