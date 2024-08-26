#include "Server.hpp"
#include "Location.hpp"
#include <iostream>
#include <string>
#include <sstream>  
#include <fstream>         
#include <vector>   

void printLocationDetails(const Location& location) {
    std::cout << "Location Details:" << std::endl;
    std::cout << "  Redirect: " << location.redirect << std::endl;
    std::cout << "  Location Path: " << location._location << std::endl;
    std::cout << "  Root: " << location._root << std::endl;
    
    std::cout << "  Error Pages:" << std::endl;
    for (const auto& [code, page] : location._errorPage) {
        std::cout << "    Code: " << code << ", Page: " << page << std::endl;
    }

    std::cout << "  Client Max Body Size: " << location._clientMaxBodySize << std::endl;

    std::cout << "  Allowed Methods: ";
    for (const auto& method : location._allowedMethods) {
        std::cout << method << " ";
    }
    std::cout << std::endl;

    std::cout << "  CGI:" << std::endl;
    for (const auto& [key, value] : location._cgi) {
        std::cout << "    " << key << " : " << value << std::endl;
    }

    std::cout << "  Upload Directory: " << location._uploadDir << std::endl;
    std::cout << "  Permit Upload String: " << location.permitUploadStr << std::endl;
    std::cout << "  Permit Upload: " << (location._permitUpload ? "Yes" : "No") << std::endl;
    std::cout << "  Return: " << location._return << std::endl;
}

void printServerDetails(const Server& server) {
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



std::string trim(const std::string& str) 
{
    size_t start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) 
		return ""; // No content

    size_t end = str.find_last_not_of(" \t\n\r");
    return str.substr(start, end - start + 1);
}

// Function to safely convert string to unsigned long
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

// Function to parse client_max_body_size from a line
unsigned long parseClientBodyMaxSize(const std::string& line) 
{
    size_t pos = 0;
    std::string valueStr;
    
    // Loop through the line to find the numeric part
    while (pos < line.size()) 
	{
        // Skip any non-digit characters
        while (pos < line.size() && !std::isdigit(line[pos])) 
            ++pos;
        // If we find digits, start extracting the number
        if (pos < line.size() && std::isdigit(line[pos])) 
		{
            size_t start = pos;
            // Continue until we reach the end of digits
            while (pos < line.size() && std::isdigit(line[pos])) 
                ++pos;
            valueStr = line.substr(start, pos - start);
            break;
        }
        
        // Move to the next character if not a digit
        ++pos;
    }

    // Check if we successfully extracted a numeric part
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

// Function to parse a Location block
void parseLocationBlock(std::istringstream& stream, Location& location) {
    std::string line;
    while (std::getline(stream, line)) {
        line = trim(line);
        if (line == "}") break; // End of location block

        if (line.find("root") == 0)
            location._root = trim(line.substr(line.find(' ') + 1));
        else if (line.find("error_page") == 0) {
            std::istringstream errorStream(trim(line.substr(line.find(' ') + 1)));
            int code;
            std::string page;
            errorStream >> code >> page;
            location._errorPage[code] = page;
        } else if (line.find("client_max_body_size") == 0)
            location._clientMaxBodySize = parseClientBodyMaxSize(trim(line.substr(line.find(' ') + 1)));
        else if (line.find("allowed_methods") == 0) {
            std::istringstream methodsStream(trim(line.substr(line.find(' ') + 1)));
            std::string method;
            while (methodsStream >> method)
                location._allowedMethods.push_back(method);
        } else if (line.find("cgi") == 0) {
            std::istringstream cgiStream(trim(line.substr(line.find(' ') + 1)));
            std::string key, value;
            while (cgiStream >> key >> value)
                location._cgi[key] = value;
        } else if (line.find("upload_dir") == 0)
            location._uploadDir = trim(line.substr(line.find(' ') + 1));
        else if (line.find("return") == 0)
            location._return = trim(line.substr(line.find(' ') + 1));
    }
}


// Function to parse a Server block
void parseServerBlock(std::istringstream& stream, Server& server) {
    std::string line;
    while (std::getline(stream, line)) {
        line = trim(line);
        if (line == "}") break;

        if (line.find("server_name") == 0)
            server._serverName = trim(line.substr(line.find(' ') + 1));
        else if (line.find("listen") == 0) {
            std::istringstream listenStream(trim(line.substr(line.find(' ') + 1)));
            std::string ipAddress, port;
            getline(listenStream, ipAddress, ':');
            getline(listenStream, port);
            server._listen.insert({ipAddress, port});
        } else if (line.find("root") == 0)
            server._root = trim(line.substr(line.find(' ') + 1));
        else if (line.find("index") == 0) {
            std::istringstream indexStream(trim(line.substr(line.find(' ') + 1)));
            std::string index;
            while (indexStream >> index)
                server._index += index + " "; // Assuming multiple indexes are space-separated
            if (!server._index.empty())
                server._index.pop_back(); // Remove trailing space
        } else if (line.find("error_page") == 0) {
            std::istringstream errorStream(trim(line.substr(line.find(' ') + 1)));
            int code;
            std::string page;
            errorStream >> code >> page;
            server._errorPage[code] = page;
        } else if (line.find("client_max_body_size") == 0)
            server._clientMaxBodySize = parseClientBodyMaxSize(trim(line.substr(line.find(' ') + 1)));
        else if (line.find("allowed_methods") == 0) {
            std::istringstream methodsStream(trim(line.substr(line.find(' ') + 1)));
            std::string method;
            while (methodsStream >> method)
                server._allowedMethods.push_back(method);
        } else if (line.find("cgi") == 0) {
            std::istringstream cgiStream(trim(line.substr(line.find(' ') + 1)));
            std::string key, value;
            while (cgiStream >> key >> value)
                server._cgi[key] = value;
        } else if (line.find("return") == 0)
            server._return = trim(line.substr(line.find(' ') + 1));
        else if (line.find("location") == 0) {
            std::string locationPath = trim(line.substr(line.find(' ') + 1));
            Location location;
            parseLocationBlock(stream, location);
            server._location[locationPath] = location;
        }
    }
}



// Function to read and parse the configuration file
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
                servers.push_back(server);
                server = Server(); // Reset for next server block
            }
        } 
        else if (inServerBlock) 
        {
            blockContent += line + "\n";
        }
    }
    file.close();
}


int main() 
{
    std::vector<Server> servers;
    parseConfigFile("default.conf", servers);

    if (servers.empty()) 
        std::cout << "No servers found or error parsing config file." << std::endl;
	else 
	{
        for (const auto& server : servers) 
			printServerDetails(server);
    }
    return 0;
}
// int	main(int argc, char **argv)
// {
// 	std::string	config;

// 	try
// 	{
// 		if (argc == 1)
// 			file_open_read(CONFIG_DEFAULT, config);
// 		else if (argc == 2)
// 			file_open_read(argv[1], config);
// 		else
// 			throw std::runtime_error("Error: Argument count must be 1 or 0.");
// 		start(config);
// 	}
// 	catch (const std::exception &e)
// 	{
// 		std::cerr << e.what() <<  std::endl;
// 	}
// 	return (0);
// }
