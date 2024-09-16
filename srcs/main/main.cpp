#include "serverheaders.hpp"
#include "Manager.hpp"
#include "Parsing.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
// #include "webserv.hpp"
using namespace std;


typedef std::string t_str;


// // Once a server is created, these status codes will be assigned.
// void assignStatusCodes(std::map<t_str, t_str>& statusCodes) {
// 	statusCodes["200"] = "OK";
// 	statusCodes["201"] = "Created";
// 	statusCodes["202"] = "Accepted";
// 	statusCodes["204"] = "No Content";
// 	statusCodes["301"] = "Moved Permanently";
// 	statusCodes["302"] = "Found";
// 	statusCodes["304"] = "Not Modified";
// 	statusCodes["400"] = "Bad Request";
// 	statusCodes["401"] = "Unauthorized";
// 	statusCodes["403"] = "Forbidden";
// 	statusCodes["404"] = "Not Found";
// 	statusCodes["405"] = "Method Not Allowed";
// 	statusCodes["408"] = "Request Timeout";
// 	statusCodes["412"] = "Precondition Failed";
// 	statusCodes["413"] = "Content Too Large";
// 	statusCodes["500"] = "Internal Server Error";
// 	statusCodes["501"] = "Not Implemented";
// 	statusCodes["502"] = "Bad Gateway";
// 	statusCodes["503"] = "Service Unavailable";
// }


// // This is just for testing
// void makefakestructs(std::vector<Server>& ser) { 
	
// 	// fake first server
// 	Server firstServer;
// 	firstServer._serverName = "";
//     firstServer.port = 8000;
//     firstServer.ipAddress = "127.0.0.1";
//     assignStatusCodes(firstServer.statusCodes);
//     firstServer._root = "/www/tmp";
// 	firstServer._index = "/index.html";
//     firstServer._clientMaxBodySize = 20000000;
//     firstServer._allowedMethods.push_back("GET");
// 	// firstServer._allowedMethods.push_back("POST");
// 	firstServer._allowedMethods.push_back("DELETE");
// 	firstServer._errorPage[400] = "/error_pages/400.html"; // note for instructions... full path will be concantenated with root
// 	firstServer._errorPage[404] = "/error_pages/404.html";
// 	// firstServer._errorPage[408] = "/error_pages/408.html";
//     firstServer.serverptr = new SimpleServer(AF_INET, SOCK_STREAM, 0, firstServer.port, firstServer.ipAddress , 5);
// 	int optval = 1;
// 	if (setsockopt(firstServer.serverptr->get_socket()->get_sock(), SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
// 		std::cerr << "Error: setsockopt failed" << std::endl;

	  

// 	// create locations for first server
// 	Location home;
// 	std::string loc = "/home";
// 	home._location = loc;
// 	home._redirect = "";
// 	home._root ="/www/tmp/locationtest";
// 	home._errorPage[400] = "/400custom.html";
// 	home._errorPage[404] = "/404custom.html";
// 	home._clientMaxBodySize = 1000000;
// 	home._allowedMethods.push_back("GET");
// 	home._allowedMethods.push_back("POST");
// 	home._allowedMethods.push_back("DELETE");
// 	home._cgi["sh"] = "/bin/bash";
// 	home._cgi["py"] = "/bin/python3";
// 	home._uploadDir = "/uploads";
// 	home._permitUpload = true;
// 	home._locationIndex = "/home.html"; // handle if this is missing and no redirect. should not need index if redirect exists and redirect has index 
// 	home._listDirectory = true; // incompatible with _locationIndex
// 	firstServer._location.push_back(home);

// 	Location upload;
// 	loc = "/upload";
// 	upload._location = loc;
// 	upload._redirect = "";
// 	upload._root ="/www/tmp";
// 	upload._errorPage[400] = "/400custom.html";
// 	upload._errorPage[404] = "/404custom.html";
// 	upload._clientMaxBodySize = 1000000000;
// 	upload._allowedMethods.push_back("GET");
// 	upload._allowedMethods.push_back("POST");
// 	upload._allowedMethods.push_back("DELETE");
// 	upload._cgi["sh"] = "/bin/bash";
// 	upload._cgi["py"] = "/bin/python3";
// 	upload._uploadDir = "/uploads";
// 	upload._permitUpload = true;	
// 	upload._permitDelete = false;
// 	upload._locationIndex = "/upload.html";
// 	upload._listDirectory = true;
// 	firstServer._location.push_back(upload);


// 	Location del;
// 	loc = "/del"; //must be preceded with /
// 	del._location = loc;
// 	del._redirect = "";
// 	del._root ="/www/tmp";
// 	del._errorPage[400] = "/locationtest//400custom.html";
// 	del._errorPage[404] = "/locationtest//404custom.html";
// 	del._clientMaxBodySize = 1000000000;
// 	del._allowedMethods.push_back("GET");
// 	del._allowedMethods.push_back("POST");
// 	del._allowedMethods.push_back("DELETE");
// 	del._uploadDir = "/uploads";
// 	del._permitDelete = true;
// 	del._locationIndex = "/delete-file.html";
// 	// add location struct to first server
// 	firstServer._location.push_back(del);

// 	Location files;
// 	loc = "/files"; //must be preceded with /
// 	files._location = loc;
// 	files._redirect = "";
// 	files._root ="/www/tmp";
// 	files._clientMaxBodySize = 1000000000;
// 	files._allowedMethods.push_back("GET");
// 	files._listDirectory = true;
// 	files._locationIndex = "/uploads";
// 	files._errorPage[404] = "/locationtest/404custom.html";
// 	// add location struct to first server
// 	firstServer._location.push_back(files);

// 	Location directory;
// 	loc = "/directory"; //must be preceded with /
// 	directory._location = loc;
// 	directory._redirect = "";
// 	directory._root ="/www/tmp";
// 	directory._clientMaxBodySize = 1000000000;
// 	directory._allowedMethods.push_back("GET");
// 	directory._listDirectory = true;
// 	directory._locationIndex = "/RandomDirectory";
// 	directory._errorPage[404] = "/locationtest/404custom.html";
// 	// add location struct to first server
// 	firstServer._location.push_back(directory);


// 	Location cgibin;
// 	loc = "/cgi-bin"; //must be preceded with /
// 	cgibin._location = loc;
// 	cgibin._redirect = "";
// 	cgibin._root ="/www/tmp";
// 	cgibin._clientMaxBodySize = 1000000000;
// 	cgibin._allowedMethods.push_back("GET");
// 	cgibin._allowedMethods.push_back("POST");
// 	cgibin._listDirectory = true;
// 	cgibin._locationIndex = "/cgi-bin";
// 	cgibin._errorPage[408] = "/error_pages/408.html";
// 	cgibin._cgi["sh"] = "/bin/bash";
// 	cgibin._cgi["py"] = "/usr/local/bin/python3";
// 	firstServer._location.push_back(cgibin);

// 	ser.push_back(firstServer);

// 	// fake second server
// 	Server secondServer;
// 	secondServer._serverName = "";
//     secondServer.port = 8001;
//     secondServer.ipAddress = "127.0.0.1";
//     assignStatusCodes(secondServer.statusCodes);
//     secondServer._root = "/www/html";
//     secondServer._clientMaxBodySize = 200000;
//     secondServer._allowedMethods.push_back("GET");
// 	secondServer._allowedMethods.push_back("POST");
//     secondServer.serverptr = new SimpleServer(AF_INET, SOCK_STREAM, 0, secondServer.port, secondServer.ipAddress , 5);
// 	setsockopt(secondServer.serverptr->get_socket()->get_sock(), SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
// 	ser.push_back(secondServer);

// 	// fake third server
// 	Server thirdServer;
// 	thirdServer._serverName = "thirdServer";
//     thirdServer.port = 8000;
//     thirdServer.ipAddress = "127.0.0.1";
//     assignStatusCodes(thirdServer.statusCodes);
//     thirdServer._root = "/www/html";
//     thirdServer._clientMaxBodySize = 200000;
//     thirdServer._allowedMethods.push_back("GET");
// 	thirdServer._allowedMethods.push_back("POST");
//     // thirdServer.serverptr = new SimpleServer(AF_INET, SOCK_STREAM, 0, thirdServer.port, thirdServer.ipAddress , 5);
// 	// setsockopt(thirdServer.serverptr->get_socket()->get_sock(), SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
// 	thirdServer.serverptr = firstServer.serverptr; // This third server is sharing the port of the first server!!
// 	ser.push_back(thirdServer);
// }

// void	start(std::string config)
// {
// 	(void)config;

// 	std::vector<Server> servers;
// 	makefakestructs(servers); // this is for testing purposes...
// 	try
// 	{
// 	  Manager *manager = new Manager(servers);
// 	  delete manager;
// 	}
// 	catch (const std::exception &e)
// 	{
// 		std::cout << e.what() << std::endl;
// 	}
	
	
// }

// int	main(int argc, char **argv)
// {
// 	std::string	config;
// 	(void)argc;
// 	(void)argv;
// 	try {
// 		start(config);
// 	} catch (const std::exception &e) {
// 		std::cerr << e.what() <<  std::endl;
// 	}
// 	return (0);
// }

// // // int main() 
// // // {

// // //     int port = 8082;
// // //     std::string ipAddress = "127.0.0.1";

// // //     SimpleServer *t = new SimpleServer(AF_INET, SOCK_STREAM, 0, port, ipAddress , 5);

    
// // //     delete t;
// // // }


int main(int argc, char **argv) 
{
	if (argc == 1)
		return (0);
    vector<Server> servers;
    parseConfigFile(argv[1], servers);

    // Clean the data
    cleanDataServers(servers);

    // initialise sockets...
    // this is just to test with first server block
    servers[0].serverptr = new SimpleServer(AF_INET, SOCK_STREAM, 0, servers[0].port, servers[0].ipAddress , 5);

    //this is to test with multiple server blocks
    // for (size_t i =0 ; i < servers.size(); i++)
	// {
    //     servers[i].serverptr = new SimpleServer(AF_INET, SOCK_STREAM, 0, servers[i].port, servers[i].ipAddress , 5);
    // }

    try
    {
        Manager *manager = new Manager(servers);
        delete manager;
    }
    catch (const std::exception &e)
    {
        std::cout << e.what() << std::endl;
    }  
    // Output parsed data (for testing purposes)
    // for (size_t i = 0; i < servers.size(); ++i) 
    // {
    //     const Server& s = servers[i];
    //     cout << "Server " << i + 1 << ":\n";
    //     cout << "  Server Name: " << s._serverName << endl;
    //     cout << "  ipAddress: " << s.ipAddress << endl;
    //     cout << "  _root: " << s._root << endl;
    //     cout << "  _index: " << s._index << endl;
    //     for (const auto& ep : s._errorPage) 
	// 	{
    //         cout << "  Error Page " << ep.first << ": " << ep.second << endl;
    //     }
    //     cout << "  Allow Methods: " << s.allow_methods << endl;
    //     cout << "  Client Max Body Size: " << s._clientMaxBodySize << endl;

    //     // Display allowed methods vector for Server
    //     cout << "  Allowed Methods Vector: ";
    //     for (const string& method : s._allowedMethods) {
    //         cout << "s.[" << method << "] ";
    //     }
    //     cout << endl;
    //     cout << "[]" << s._location.size() << "[]" << std::endl;
    //     for (size_t j = 0; j < s._location.size(); ++j) 
    //     {
    //         const Location& l = s._location[j];
    //         cout << "  Location " << j + 1 << ":\n";
    //         cout << "    Path: " << l._location << endl;
    //         cout << "    _redirect: " << l._redirect << endl;
    //         cout << "    _root: " << l._root << endl;
    //         for (const auto& ep : l._errorPage) 
    //         {
    //             cout << "    Error Page " << ep.first << ": " << ep.second << endl;
    //         }
    //         cout << "    Client Max Body Size: " << l._clientMaxBodySize << endl;

    //         // Display allowed methods vector for Location
    //         cout << "    Allowed Methods Vector: ";
    //         for (const string& method : l._allowedMethods) 
    //         {
    //             cout << "l.[" << method << "] ";
    //         }
    //         cout << endl;

    //         for (const auto& cgi : l._cgi) 
    //         {
    //             cout << "    CGI " << cgi.first << ": " << cgi.second << endl;
    //         }
    //         cout << "    Upload Dir: " << l._uploadDir << endl;
    //         cout << "    Permit Upload: " << (l._permitUpload ? "on" : "off") << endl;
    //         cout << "    Permit Delete: " << (l._permitDelete ? "on" : "off") << endl;
    //         cout << "    Permit Directory: " << (l._listDirectory ? "on" : "off") << endl;

    //     }
    // }

    return 0;
}





