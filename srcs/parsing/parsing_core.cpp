#include "Parsing.hpp"


void cleanDataLoc(Location &loc) 
{
    cleanStringCopy(loc._redirect);
    cleanStringCopy(loc._location);
    cleanStringCopy(loc._root);
    cleanStringCopy(loc.permitUploadStr);
    // cleanStringCopy(loc._return);
    cleanStringCopy(loc._locationIndex);
    for (std::map<int, Location::t_str>::iterator it = loc._errorPage.begin(); it != loc._errorPage.end(); ++it) 
        cleanStringCopy(it->second);
    for (std::vector<Location::t_str>::iterator it = loc._allowedMethods.begin(); it != loc._allowedMethods.end(); ++it)
        cleanStringCopy(*it);
    for (std::map<std::string, std::string>::iterator it = loc._cgi.begin(); it != loc._cgi.end(); ++it) 
	{
        cleanStringCopy(it->first);
        cleanStringCopy(it->second);
    }
    cleanStringCopy(loc._uploadDir);
};

void cleanDataServer(Server &srv) 
{
    cleanString(srv._serverName);
    cleanString(srv.ipAddress);
    cleanString(srv._root);
    cleanString(srv._index);
    cleanString(srv.allow_methods);
    std::cout << "{" << srv.allow_methods << "}" << std::endl;
    srv._allowedMethods.clear();
    std::istringstream iss(srv.allow_methods.c_str());
    std::string method;
    while (iss >> method) 
    {
        cleanString(method);
        srv._allowedMethods.push_back(method);
    }
    for (std::map<int, std::string>::iterator it = srv._errorPage.begin(); it != srv._errorPage.end(); ++it) 
        cleanString(it->second);
    for (std::vector<Location>::iterator it = srv._location.begin(); it != srv._location.end(); ++it) 
        cleanDataLoc(*it);
    std::string ipAddress;
    int port = 0;
    splitIpAddressAndPort(srv.ipAddress, ipAddress, port);
    srv.ipAddress = ipAddress;
    srv.port = port;
};

void cleanDataServers(std::vector<Server> &servers)
{
    for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it) 
        cleanDataServer(*it);	
};

// Function to parse a location block
void parseLocation(std::stringstream& iss, Location& location) 
{
    std::string key, value;
    while (iss >> key) 
    {
        if (key == "redirect") 
        {
            iss >> value;
            location._redirect = value;
        } 
        else if (key == "root") 
        {
            
            iss >> value;
            std::cout << "found ==" << value << std::endl;
            location._root = value;
        } 
        else if (key == "error_page") 
        {
            int code;
            std::string page;
            iss >> code >> page;
            location._errorPage[code] = page;
        } 
        else if (key == "client_max_body_size") 
        {
            iss >> value;
            location._clientMaxBodySize = std::stoul(value);
        } 
        else if (key == "allow_methods") 
        {
            std::string method;
            location._allowedMethods.clear(); // Ensure old methods are removed
            while (iss >> method) 
            {
                if (method == "POST" || method == "GET" || method == "DELETE") 
                {
                    location._allowedMethods.push_back(method);
                }
                else
                {
                    break; // End of allowed methods list
                }
            }
        }
        else if (key == "cgi") 
        {
            std::string type, path;
            iss >> type >> path;
            location._cgi[type] = path;
        } 
        else if (key == "upload_dir") 
        {
            iss >> value;
            location._uploadDir = value;
        } 
        else if (key == "permit_upload") 
        {
            iss >> value;
            location._permitUpload = (value != "on");
        } 
        else if (key == "permit_delete") 
        {
            iss >> value;
            location._permitDelete = (value != "on");
        }
        else if (key == "permit_directory") 
        {
            iss >> value;
            location._listDirectory = (value != "on");
        }
        else if (key == "}") 
        {
            break; // End of location block
        }
    }
}

void parseLine(const std::string& line, Server& server, Location& location, bool& inLocation, std::stringstream& locationStream) 
{
    std::stringstream iss(line);
    std::string key, value;

    if (line.find("location") != std::string::npos) 
    {
        if (inLocation) 
        {
        // Parse and store the previous location
            parseLocation(locationStream, location);
            server._location.push_back(location);
        }
    // Start a new location block
        inLocation = true;
        locationStream.str(""); // Clear the stream
        locationStream.clear(); // Clear the stream state
    // Extract and store the path from the line
        location._location = line.substr(line.find('/') + 1); 
    // Reset location (but keep path)
        location = Location(); 
    }
    else if (line == "}") 
    {
        if (inLocation) 
        {
        // End of location block
            parseLocation(locationStream, location);
            server._location.push_back(location);
            inLocation = false; // End location mode
        // Reset only necessary fields for next use
            location._redirect.clear();
            location._root.clear();
            location._errorPage.clear();
        // Add other fields as needed...
        }
    }
    else 
    {
        if (inLocation) 
        {
            locationStream << line << '\n'; // Append line to locationStream
        } 
        else 
        {
            iss >> key;
            if (key == "server_name") 
            {
                iss >> value;
                server._serverName = value;
            } 
            else if (key == "listen") 
            {
                iss >> value;
                server.ipAddress = value;
            } 
            else if (key == "root") 
            {
                iss >> value;
                server._root = value;
            } 
            else if (key == "index") 
            {
                iss >> value;
                server._index = value;
            } 
            else if (key == "error_page") 
            {
                int code;
                std::string page;
                iss >> code >> page;
                if (code == 400 || code == 404 || code == 500) 
                {
                    server._errorPage[code] = page;
                }
            } 
            else if (key == "client_max_body_size") 
            {
                iss >> value;
                server._clientMaxBodySize = std::stoul(value);
            } 
            else if (key == "allow_methods") 
            {
                server.allow_methods.clear();
                std::string method;
                while (iss >> method) 
                {
                    if (method == "POST" || method == "GET" || method == "DELETE") 
                    {
                        server.allow_methods += method + " ";
                    }
                }
                server.allow_methods = trim(server.allow_methods); // Assuming trim is a function you have
            }
            // The 'cgi' block was commented out in your original code, if needed, uncomment and adjust
        }
    }
}


void parseConfigFile(const string& filename, vector<Server>& servers)
{
    ifstream file(filename.c_str());
    if (!file.is_open()) 
    {
        cerr << "Error opening file: " << filename << endl;
        return;
    }
    string line;
    Server server;
    Location location;
    bool inLocation = false;
    std::stringstream locationStream;
    while (getline(file, line)) 
    {
        if (line.empty() || line[0] == '#') 
            continue;
        if (line.find("server {") != string::npos)
        {
            server = Server(); // Reset server for new block
        }
        else if (line == "}")
        {
            if (inLocation) // Ensure all locations are closed before pushing server
            {
                parseLocation(locationStream, location);
                server._location.push_back(location);
                inLocation = false;
            }
            servers.push_back(server); // Push completed server block
        }
        else 
        {
            parseLine(line, server, location, inLocation, locationStream);
        }
    }
};
