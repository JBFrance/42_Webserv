#include "Client.hpp"


Client::Client(int sock, struct sockaddr_in addr,  std::vector<Server> servers, const Server* listeningserver)
        : socket(sock), address(addr),  servers(servers), listeningserver(listeningserver) {
        this->payloadSize = -1;
        this->postDetected = 0;
        this->maxContentLength = listeningserver->_clientMaxBodySize;
        this->clientStatusCode = 200;
    
            
    }

void Client::createRequest() {
    this->request = new Request(servers, requestBuffer.c_str(), listeningserver);
    this->request->currentClientPtr = this; 
}

void Client::createResponse() {
    this->res = new Response(request, servers, this->socket, listeningserver);
  
}

int stringToInt(const std::string& str) {
    std::stringstream ss(str);
    int result;
    ss >> result;
    return result;
}

void Client::getPayloadLength() {
    if (!postDetected && payloadSize < 0) {
        size_t pos = requestBuffer.find("Content-Length: ");
        if (pos != std::string::npos) {
            pos += strlen("Content-Length: ");
            size_t endPos = requestBuffer.find("\r\n", pos);
            if (endPos != std::string::npos) {
                std::string lengthStr = requestBuffer.substr(pos, endPos - pos);
                payloadSize = stringToInt(lengthStr);
            }
            postDetected = 1;
        }
    }

     
}



Client::~Client() {
    requestBuffer.clear();
    std::string emptyBuffer = "";
    requestBuffer.swap(emptyBuffer);

    delete this->request;
    delete this->res;
    close(socket);
}