#include "Server.hpp"
#include "Location.hpp"

int countWords(const std::string& str) 
{
    std::istringstream iss(str);
    std::string word;
    int count = 0;

    while (iss >> word) 
        count++;
    return count;
}

std::string trimm(const std::string& str) //wip
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

std::string getSecondWord(const std::string& str) 
{
    std::istringstream iss(str);
    std::string word;
    std::string firstWord;
    std::string secondWord;

    // Extract words from the stream
    if (iss >> firstWord && iss >> secondWord) 
        return secondWord;
    else 
        return "";
}

Location cleanDataLocation(Location location)
{
    std::cout << "despues: " << location.redirect << std::endl;
    location.redirect = getSecondWord(location.redirect);
    std::cout << "antes: " << location.redirect << std::endl;
    return location;
}

Server cleanData(Server server)
{
    //put word_count and error handling here
    server._serverName = getSecondWord(server._serverName);
    server._root = getSecondWord(server._root);
    //server._index = getSecondWord(server._index);
    std::map<std::string, Location>::iterator it;
    for (std::size_t i = 0; i < server._location.size(); ++i) 
        server._location[i] = cleanDataLocation(server._location[i]);
    std::cout << "2: " << server._clientMaxBodySize << std::endl;
    return server;
}