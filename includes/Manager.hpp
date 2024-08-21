#ifndef __MANAGER_HPP__
# define __MANAGER_HPP__

# include "managerheaders.hpp"

class Client;

class Manager
{
    protected:
        std::vector<Server>& servers;
        fd_set current_sockets, ready_sockets;
        int extract_content_length(const std::string& headers) ;
        int setNonBlocking(int socket) ;
        void addServerSockets(std::vector<Server>& server, int &maxsocket);
        void addNewSocket(Server &listeningserver, int &maxSocket);
        int stringToInt(const std::string& str);
        fd_set readSet;
        fd_set writeSet;
        fd_set read_master;
        fd_set write_master;
        std::vector<Client *> openClients;

    public:
        Manager(std::vector<Server>& servers);
        ~Manager();
        void listen_for_events();

};

#endif