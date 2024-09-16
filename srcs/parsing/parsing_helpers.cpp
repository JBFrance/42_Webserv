#include "Parsing.hpp"

void splitIpAddressAndPort(const std::string &str, std::string &ipAddress, int &port)
{
    size_t pos = str.find(':');
    if (pos != std::string::npos) 
	{
        ipAddress = str.substr(0, pos);
        std::string portStr = str.substr(pos + 1);
        std::istringstream portStream(portStr);
        portStream >> port;
    } 
	else 
	{
        ipAddress = str;
        port = -1;
    }
}


std::string cleanStringCopy(const std::string &str)
{
    std::string cleaned = str;
    size_t pos = cleaned.find(';');
    if (pos != std::string::npos) 
	{
        cleaned = cleaned.substr(0, pos);
    }
    return cleaned;
}


void cleanString(std::string &str)
{
    size_t pos = str.find(';');
    if (pos != std::string::npos) 
	{
        str = str.substr(0, pos);
    }
}

std::string trim(const std::string& str)
{
    size_t start = str.find_first_not_of(" \t");
    size_t end = str.find_last_not_of(" \t");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}