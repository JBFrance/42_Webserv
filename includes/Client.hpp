#ifndef __CLIENT_HPP__
#define __CLIENT_HPP__

# include <string>
# include <vector>
# include <map>
# include "Manager.hpp"
# include "Request.hpp"
# include "Response.hpp"

class Request;
class Response;

class Client {
    public:
        int socket;
        int postDetected;
        long payloadSize;
        long maxContentLength;
        std::string ipAddress;
        struct sockaddr_in address;
        std::string requestBuffer;
        Request *request;
        Response *res;
        std::vector<Server> servers;
        const Server* listeningserver;
        int clientStatusCode;

        Client(int sock, struct sockaddr_in addr, std::vector<Server> servers, const Server* listeningserver);
        void createRequest();
        void createResponse();
        void getPayloadLength();

        ~Client();
    };

#endif 