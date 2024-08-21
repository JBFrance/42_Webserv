#include "serverheaders.hpp"
#include "webserv.hpp"
#include "Manager.hpp"


void printServerDetails(const Server& server) 
{
    // Display server basic information
    std::cout << "Server IP: " << server.ipAddress << ", Port: " << server.port << std::endl;

    // Display uploadLocation and deleteLocation
    std::cout << "Upload Location: " << server.uploadLocation << std::endl;
    std::cout << "Delete Location: " << server.deleteLocation << std::endl;

    // Display directive list
    std::cout << "Directive List:" << std::endl;
    for (const auto& directive : server._directiveList) 
	{
        std::cout << "  " << directive << std::endl;
    }

    // Display server names
    std::cout << "Server Names:" << std::endl;
    for (const auto& name : server._serverName) 
	{
        std::cout << "  " << name << std::endl;
    }

    // Display listen directives
    std::cout << "Listen Directives:" << std::endl;
    for (const auto& pair : server._listen) 
	{
        std::cout << "  " << pair.first << " => " << pair.second << std::endl;
    }

    // Display location mappings
    std::cout << "Locations:" << std::endl;
    for (const auto& locPair : server._location) 
	{
        std::cout << "  Location path: " << locPair.first << std::endl;
        const Location& location = locPair.second;
        
        // Display location-specific details
        std::cout << "    Root: " << location._root << std::endl;
        std::cout << "    Indexes:" << std::endl;
        for (const auto& index : location._index) 
		{
            std::cout << "      " << index << std::endl;
        }
        std::cout << "    Autoindex: " << (location._autoindex ? "on" : "off") << std::endl;
        std::cout << "    Is Autoindexed: " << (location._isAutoindexed ? "yes" : "no") << std::endl;
        std::cout << "    Error Pages:" << std::endl;
        for (const auto& errorPair : location._errorPage) 
		{
            std::cout << "      " << errorPair.first << " => " << errorPair.second << std::endl;
        }
        std::cout << "    Client Max Body Size: " << location._clientMaxBodySize << std::endl;
        std::cout << "    Allowed Methods:" << std::endl;
        for (const auto& method : location._allowedMethods) 
		{
            std::cout << "      " << method << std::endl;
        }
        std::cout << "    CGI:" << std::endl;
        for (const auto& cgiPair : location._cgi) 
		{
            std::cout << "      " << cgiPair.first << " => " << cgiPair.second << std::endl;
        }
        std::cout << "    Upload Directory: " << location._uploadDir << std::endl;
        std::cout << "    Delete Location: " << location.deleteLocation << std::endl;
        std::cout << "    Return: " << location._return << std::endl;
    }

    // Display root
    std::cout << "Root: " << server._root << std::endl;

    // Display indexes
    std::cout << "Indexes:" << std::endl;
    for (const auto& index : server._index) 
	{
        std::cout << "  " << index << std::endl;
    }

    // Display autoindex
    std::cout << "Autoindex: " << (server._autoindex ? "on" : "off") << std::endl;

    // Display error pages
    std::cout << "Error Pages:" << std::endl;
    for (const auto& errorPair : server._errorPage) 
	{
        std::cout << "  " << errorPair.first << " => " << errorPair.second << std::endl;
    }

    // Display client max body size
    std::cout << "Client Max Body Size: " << server._clientMaxBodySize << std::endl;

    // Display allowed methods
    std::cout << "Allowed Methods:" << std::endl;
    for (const auto& method : server._allowedMethods) 
	{
        std::cout << "  " << method << std::endl;
    }

    // Display CGI directives
    std::cout << "CGI Directives:" << std::endl;
    for (const auto& cgiPair : server._cgi) 
	{
        std::cout << "  " << cgiPair.first << " => " << cgiPair.second << std::endl;
    }

    // Display return status
    std::cout << "Return: " << server._return << std::endl;

    // Display upload directory
    std::cout << "Upload Directory: " << server._uploadDir << std::endl;
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
	{
        throw std::invalid_argument("Empty string cannot be converted to unsigned long");
    }
    for (char c : trimmedStr) 
	{
        if (!std::isdigit(c)) 
		{
            throw std::invalid_argument("String contains non-numeric characters");
        }
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
		{
            ++pos;
        }
        
        // If we find digits, start extracting the number
        if (pos < line.size() && std::isdigit(line[pos])) 
		{
            size_t start = pos;
            // Continue until we reach the end of digits
            while (pos < line.size() && std::isdigit(line[pos])) 
			{
                ++pos;
            }
            valueStr = line.substr(start, pos - start);
            break;
        }
        
        // Move to the next character if not a digit
        ++pos;
    }

    // Check if we successfully extracted a numeric part
    if (valueStr.empty()) 
	{
        throw std::invalid_argument("No numeric value found in the input line");
    }
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
void parseLocationBlock(std::istringstream& stream, Location& location) 
{
    std::string line;
    while (std::getline(stream, line)) 
	{
        line = trim(line);
        if (line == "}") break; // End of location block

        // Parse the directives inside location
        if (line.find("root") == 0) 
		{
            location._root = trim(line.substr(line.find(' ') + 1));
        } 
		else if (line.find("index") == 0) 
		{
            std::istringstream indexStream(trim(line.substr(line.find(' ') + 1)));
            std::string index;
            while (indexStream >> index) 
			{
                location._index.push_back(index);
            }
        } 
		else if (line.find("autoindex") == 0) 
		{
            location._autoindex = (trim(line.substr(line.find(' ') + 1)) == "on");
        } 
		else if (line.find("error_page") == 0) 
		{
            std::istringstream errorStream(trim(line.substr(line.find(' ') + 1)));
            int code;
            std::string page;
            errorStream >> code >> page;
            location._errorPage[code] = page;
        } 
		else if (line.find("client_max_body_size") == 0) 
		{
            location._clientMaxBodySize = parseClientBodyMaxSize(trim(line.substr(line.find(' ') + 1)));
        } 
		else if (line.find("allowed_methods") == 0) 
		{
            std::istringstream methodsStream(trim(line.substr(line.find(' ') + 1)));
            std::string method;
            while (methodsStream >> method) 
			{
                location._allowedMethods.push_back(method);
            }
        } 
		else if (line.find("cgi") == 0) 
		{
            std::istringstream cgiStream(trim(line.substr(line.find(' ') + 1)));
            std::string key, value;
            while (cgiStream >> key >> value) 
			{
                location._cgi[key] = value;
            }
        } 
		else if (line.find("upload_dir") == 0) 
		{
            location._uploadDir = trim(line.substr(line.find(' ') + 1));
        } 
		else if (line.find("delete_location") == 0) 
		{
            location.deleteLocation = trim(line.substr(line.find(' ') + 1));
        } 
		else if (line.find("return") == 0) 
		{
            location._return = trim(line.substr(line.find(' ') + 1));
        }
    }
}

// Function to parse a Server block
void parseServerBlock(std::istringstream& stream, Server& server) 
{
    std::string line;
    while (std::getline(stream, line)) 
	{
        line = trim(line);
        if (line == "}") break; // End of server block

        // Parse the directives inside server
        if (line.find("server_name") == 0) 
		{
            server._serverName.push_back(trim(line.substr(line.find(' ') + 1)));
        } 
		else if (line.find("listen") == 0) 
		{
            std::istringstream listenStream(trim(line.substr(line.find(' ') + 1)));
            std::string ipAddress;
            std::string port;
            listenStream >> ipAddress >> port;
            server._listen.insert({ipAddress, port});
        }
		else if (line.find("root") == 0) 
		{
            server._root = trim(line.substr(line.find(' ') + 1));
        } 
		else if (line.find("index") == 0) 
		{
            std::istringstream indexStream(trim(line.substr(line.find(' ') + 1)));
            std::string index;
            while (indexStream >> index) 
			{
                server._index.push_back(index);
            }
        } 
		else if (line.find("autoindex") == 0) 
		{
            server._autoindex = (trim(line.substr(line.find(' ') + 1)) == "on");
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
		{
            server._clientMaxBodySize = parseClientBodyMaxSize(trim(line.substr(line.find(' ') + 1)));
        } 
		else if (line.find("allowed_methods") == 0) 
		{
            std::istringstream methodsStream(trim(line.substr(line.find(' ') + 1)));
            std::string method;
            while (methodsStream >> method) 
			{
                server._allowedMethods.push_back(method);
            }
        } 
		else if (line.find("cgi") == 0) 
		{
            std::istringstream cgiStream(trim(line.substr(line.find(' ') + 1)));
            std::string key, value;
            while (cgiStream >> key >> value) 
			{
                server._cgi[key] = value;
            }
        } 
		else if (line.find("upload_location") == 0) 
		{
            server.uploadLocation = trim(line.substr(line.find(' ') + 1));
        } 
		else if (line.find("delete_location") == 0) 
		{
            server.deleteLocation = trim(line.substr(line.find(' ') + 1));
        } 
		else if (line.find("return") == 0) 
		{
            server._return = trim(line.substr(line.find(' ') + 1));
        } 
		else if (line == "location {") 
		{
            Location location;
            parseLocationBlock(stream, location);
            // Assuming location block is associated with a path
            std::string locationPath = trim(line.substr(0, line.find('{')));
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
        if (line == "server {") 
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

    // Debugging output
    if (servers.empty()) {
        std::cout << "No servers found or error parsing config file." << std::endl;
    } 
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
