#ifndef __RESPONSE_HPP__
# define __RESPONSE_HPP__

# include <fstream>
# include <sstream>
# include <stdio.h>
# include <stdlib.h>
# include "Request.hpp"

class Request;
struct Server;

class Response {
private:
    std::string RESET; 
    std::string RED;
    std::string BLUE; 

    Response();
    Request *request;
    std::vector<Server> servers;
    int i;
    const Server* listeningserver;
    template <typename T> std::string to_string(T value);
    std::string getContentType(const std::string& fileName);
    std::string getExtType(const std::string& fileName);
    void runCGI();

public:
    void    writeStamp();
    int statusCode;
    void    deliverErrorPage(int fd, int statusCode, Server* queriedServer);
    void servePage();
    Response(Request *request, std::vector<Server>& servers, const int i, const Server* listeningserver);
    ~Response();

};

#endif