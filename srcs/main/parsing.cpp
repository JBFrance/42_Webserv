#include "Server.hpp"
#include "Location.hpp"

#include <iostream>
#include <string>
#include <sstream>  
#include <fstream>         
#include <vector>   

std::string trim(const std::string& str) 
{
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) 
		return "";

    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

unsigned long safeStoul(const std::string& str) 
{
    std::string trimmedStr = trim(str);
    if (trimmedStr.empty()) 
        throw std::invalid_argument("Empty string cannot be converted to unsigned long");
    for (char c : trimmedStr) 
	{
        if (!std::isdigit(c)) 
            throw std::invalid_argument("String contains non-numeric characters");
    }
    try 
	{
        return std::stoul(trimmedStr);
    } 
	catch (const std::invalid_argument& e) 
	{
        std::cerr << "Conversion error: " << e.what() << std::endl;
        throw;
    } 
	catch (const std::out_of_range& e) 
	{
        std::cerr << "Out of range error: " << e.what() << std::endl;
        throw;
    }
}

unsigned long parseClientBodyMaxSize(const std::string& line) 
{
    size_t pos = 0;
    std::string valueStr;
    
    while (pos < line.size()) 
	{
        while (pos < line.size() && !std::isdigit(line[pos])) 
            ++pos;
        if (pos < line.size() && std::isdigit(line[pos])) 
		{
            size_t start = pos;
            while (pos < line.size() && std::isdigit(line[pos])) 
                ++pos;
            valueStr = line.substr(start, pos - start);
            break;
        }
        ++pos;
    }

    if (valueStr.empty()) 
        throw std::invalid_argument("No numeric value found in the input line");
    try
    {
        return safeStoul(trim(valueStr));
    }
	catch (const std::exception& e) 
	{
        std::cerr << "Failed to parse client_max_body_size: " << e.what() << std::endl;
        throw;
    }
}

void parseLocationBlock(std::istringstream& stream, Location& location) {
    std::string line;
    while (std::getline(stream, line)) 
    {
        line = trim(line);
        if (line == "}")
            break;
        if (line.find("root") == 0)
            location._root = trim(line.substr(line.find(' ') + 1));
        else if (line.find("error_page") == 0) 
        {
            std::istringstream errorStream(trim(line.substr(line.find(' ') + 1)));
            int code;
            std::string page;
            errorStream >> code >> page;
            location._errorPage[code] = page;
        } 
        else if (line.find("client_max_body_size") == 0)
            location._clientMaxBodySize = parseClientBodyMaxSize(trim(line.substr(line.find(' ') + 1)));
        else if (line.find("allowed_methods") == 0) 
        {
            std::istringstream methodsStream(trim(line.substr(line.find(' ') + 1)));
            std::string method;
            while (methodsStream >> method)
                location._allowedMethods.push_back(method);
        } 
        else if (line.find("cgi") == 0) 
        {
            std::istringstream cgiStream(trim(line.substr(line.find(' ') + 1)));
            std::string key, value;
            while (cgiStream >> key >> value)
                location._cgi[key] = value;
        } 
        else if (line.find("upload_dir") == 0)
            location._uploadDir = trim(line.substr(line.find(' ') + 1));
        else if (line.find("return") == 0)
            location._return = trim(line.substr(line.find(' ') + 1));
    }
}

void parseServerBlock(std::istringstream& stream, Server& server) 
{
    std::string line;
    while (std::getline(stream, line)) 
    {
        line = trim(line);
        if (line == "}") 
            break;
        if (line.find("server_name") == 0)
            server._serverName = trim(line.substr(line.find(' ') + 1));
        else if (line.find("listen") == 0) 
        {
            std::istringstream listenStream(trim(line.substr(line.find(' ') + 1)));
            std::string ipAddress, port;
            getline(listenStream, ipAddress, ':');
            getline(listenStream, port);
            server._listen.insert({ipAddress, port});
        } 
        else if (line.find("root") == 0)
            server._root = trim(line.substr(line.find(' ') + 1));
        else if (line.find("index") == 0) 
        {
            std::istringstream indexStream(trim(line.substr(line.find(' ') + 1)));
            std::string index;
            while (indexStream >> index)
                server._index += index + " ";
            if (!server._index.empty())
                server._index.pop_back();
        } 
        else if (line.find("error_page") == 0) 
        {
            std::istringstream errorStream(trim(line.substr(line.find(' ') + 1)));
            int code;
            std::string page;
            errorStream >> code >> page;
            server._errorPage[code] = page;
        } 
        else if (line.find("client_max_body_size") == 0)
            server._clientMaxBodySize = parseClientBodyMaxSize(trim(line.substr(line.find(' ') + 1)));
        else if (line.find("allowed_methods") == 0) 
        {
            std::istringstream methodsStream(trim(line.substr(line.find(' ') + 1)));
            std::string method;
            while (methodsStream >> method)
                server._allowedMethods.push_back(method);
        } 
        else if (line.find("cgi") == 0) 
        {
            std::istringstream cgiStream(trim(line.substr(line.find(' ') + 1)));
            std::string key, value;
            while (cgiStream >> key >> value)
                server._cgi[key] = value;
        } 
        else if (line.find("return") == 0)
            server._return = trim(line.substr(line.find(' ') + 1));
        else if (line.find("location") == 0) 
        {
            std::string locationPath = trim(line.substr(line.find(' ') + 1));
            Location location;
            parseLocationBlock(stream, location);
            server._location[locationPath] = location;
        }
    }
}

void parseConfigFile(const std::string& filename, std::vector<Server>& servers) 
{
    std::ifstream file(filename);
    if (!file.is_open()) 
    {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }
    
    std::string line;
    std::string blockContent;
    Server server;
    bool inServerBlock = false;

    while (std::getline(file, line)) 
    {
        line = trim(line);
        if (line.empty()) 
            continue;
        if (line.find("server") != std::string::npos && line.find("{") != std::string::npos) 
        {
            inServerBlock = true;
            blockContent.clear();
        } 
        else if (line == "}") 
        {
            if (inServerBlock) 
            {
                inServerBlock = false;
                std::istringstream blockStream(blockContent);
                parseServerBlock(blockStream, server);
                server = cleanData(server);
                servers.push_back(server);
                server = Server();
            }
        } 
        else if (inServerBlock) 
        {
            blockContent += line + "\n";
        }
    }
    file.close();
}