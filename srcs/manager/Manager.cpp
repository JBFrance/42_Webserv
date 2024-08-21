

#include "Manager.hpp"
// #include "../../includes/eventheaders.hpp"
#include <fcntl.h>
#include <sstream> 

Manager::Manager(std::vector<Server>& servers): servers(servers) {
    std::string RESET = "\033[0m";
    std::string YELLOW = "\033[33m";
    FD_ZERO(&readSet);
    FD_ZERO(&writeSet);
    FD_ZERO(&read_master);
    FD_ZERO(&write_master);

    //added to test
    // servers[0].serverptr = new SimpleServer(AF_INET, SOCK_STREAM, 0, 8000, "127.0.0.1" , 5);
    // std::cout << servers[0].serverptr->get_socket()->get_sock() << "-----HERE" << std::endl;
    //

    for (unsigned int i = 0;i < servers.size(); i++) {
        fcntl(servers[i].serverptr->get_socket()->get_sock(), F_SETFL, O_NONBLOCK);
        std::cout << YELLOW << "Webserver listening on port " << servers[i].port << "..." <<  RESET << std::endl;
    }
    listen_for_events();
}

int Manager::stringToInt(const std::string& str) {
    std::istringstream iss(str);
    int result;
    iss >> result;
    return result;
}

int Manager::extract_content_length(const std::string& headers) {
    const std::string content_length_str = "Content-Length: ";
    size_t pos = headers.find(content_length_str);
    if (pos != std::string::npos) {
        size_t end_pos = headers.find("\r\n", pos);
        std::string length_str = headers.substr(pos + content_length_str.length(), end_pos - (pos + content_length_str.length()));
        return stringToInt(length_str);
    }
    return -1;
}

int Manager::setNonBlocking(int socket) {
    int flags = fcntl(socket, F_GETFL, 0);
    if (flags == -1) {
        return -1;
    }
    return fcntl(socket, F_SETFL, flags | O_NONBLOCK);
}



void Manager::addServerSockets(std::vector<Server>& servers, int &maxSocket) {
    for (size_t i = 0; i < servers.size(); i++) {
        fcntl(servers[i].serverptr->get_socket()->get_sock(), F_SETFL, O_NONBLOCK, FD_CLOEXEC);
        FD_SET(servers[i].serverptr->get_socket()->get_sock(), &read_master);
        if (servers[i].serverptr->get_socket()->get_sock() > maxSocket)
            maxSocket = servers[i].serverptr->get_socket()->get_sock();
    }
}

void Manager::addNewSocket(Server &listeningserver, int &maxSocket) {
    struct sockaddr_in address = listeningserver.serverptr->get_socket()->get_address();
    int addrlen = sizeof(address);
    int newSocket = accept(listeningserver.serverptr->get_socket()->get_sock(), (struct sockaddr *) &address, (socklen_t *)&addrlen);
    if (newSocket == - 1) {
        perror("Error accepting connection");
        return;
    }
    fcntl(newSocket, F_SETFL, O_NONBLOCK, FD_CLOEXEC);
    FD_SET(newSocket, &read_master);
    if (newSocket > maxSocket)
        maxSocket = newSocket;
    // Client *newClientSocket = new Client(newSocket, address,  listeningserver, servers);
        Client *newClientSocket = new Client(newSocket, address,  servers, &listeningserver);
        //set all values
     
     
    openClients.push_back(newClientSocket);
}

void Manager::listen_for_events() {
    int maxSocket = 0;
    addServerSockets(servers, maxSocket);
 

    while (maxSocket) {
   
        this->readSet = read_master;
		this->writeSet = write_master;

        int activity =  select(maxSocket + 1, &readSet, &writeSet, NULL, NULL);
        if (activity <= 0) {
            perror("Error in initiating select");
            continue;
        }

        for (unsigned int i = 0; i < servers.size(); i++) {
            if (FD_ISSET(servers[i].serverptr->get_socket()->get_sock(), &readSet)) {
                addNewSocket(servers[i], maxSocket);
            } 
        }
     
        for (std::vector<Client *>::iterator it = openClients.begin(); it != openClients.end(); ++it) {
            Client &currentClient = **it; //double derefence otherwise i need to create Client *
            int currentSocket = currentClient.socket;
            char buffer[1024];
        
            memset(buffer, 0, 1024);

            if (FD_ISSET(currentSocket, &readSet)) {
                int bytesRead = recv(currentSocket, buffer, 1024, 0);
                currentClient.getPayloadLength();
                if (bytesRead > 0) {
                    currentClient.getPayloadLength();
                }
                if (bytesRead == -1) {
                    delete (*it);
                    it = openClients.erase(it);
                    --it;
                    close(currentSocket);
                    FD_CLR(currentSocket, &read_master);
                } else if (bytesRead < 1024) {
                    currentClient.requestBuffer += std::string(buffer, bytesRead);
                    size_t sizeOfBuffer = currentClient.requestBuffer.size();
                    if (static_cast<long>(sizeOfBuffer) >= currentClient.payloadSize) {
                        FD_CLR(currentSocket, &read_master);
                        FD_SET(currentSocket, &write_master);
                        currentClient.createRequest();
                        currentClient.request->parseHeaders();
                        currentClient.getPayloadLength();
                        if (currentClient.postDetected) {
                            // fix uploadLocationMatch and handle error if mismatch
                            if ((currentClient.payloadSize <= currentClient.maxContentLength) && currentClient.request->uploadLocationMatch()) {
                                currentClient.request->handleUpload();
                            }
                            if (currentClient.payloadSize > currentClient.maxContentLength)
                                currentClient.clientStatusCode = 413;
                        }
                        if (currentClient.request->headers["Method"] == "DELETE")
                            currentClient.request->handleDelete();
                            // std::cout << "DeleteFound" << std::endl;
                        currentClient.createResponse();
                    } else if (currentClient.payloadSize == -1) {
                        FD_CLR(currentSocket, &read_master);
                        FD_SET(currentSocket, &write_master);
                        currentClient.createRequest();
                        currentClient.request->parseHeaders();
                        currentClient.createResponse();
                    }
                            
                } else {
                    currentClient.requestBuffer += std::string(buffer, bytesRead);;
                }
            }
            if (FD_ISSET(currentSocket, &writeSet)) {
              
                // if ((currentClient.payloadSize > currentClient.maxContentLength) || currentClient.clientStatusCode == 500) {
                if (currentClient.clientStatusCode != 200) {
                    Server* queriedServer = const_cast<Server*>(currentClient.listeningserver);
                    currentClient.res->deliverErrorPage(currentSocket, currentClient.clientStatusCode, queriedServer );
                    currentClient.clientStatusCode = 200;
                    currentClient.res->writeStamp();
                }
                else 
                    currentClient.res->servePage();
                
                delete (*it);
                it = openClients.erase(it);
                --it;
                close(currentSocket);
                
                FD_CLR(currentSocket, &write_master);

            }
        }
     
   
    
    }
}

Manager::~Manager() {

}

