#ifndef PARSING_HPP
# define PARSING_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
using namespace std;

#include "Location.hpp"
#include "Server.hpp"

void parseLocation(stringstream& iss, Location& location);
void parseLine(const string& line, Server& server, Location& location, bool& inLocation, stringstream& locationStream);
void cleanDataLoc(Location &loc);
void cleanDataServer(Server &srv);
void parseConfigFile(const string& filename, vector<Server>& servers);
void cleanDataServers(std::vector<Server> &servers);
std::string trim(const std::string& str);
void cleanString(std::string &str);
std::string cleanStringCopy(const std::string &str);
void splitIpAddressAndPort(const std::string &str, std::string &ipAddress, int &port);

#endif