    #ifndef __REQUEST_HPP__
    # define __REQUEST_HPP__

    # include <vector>
    # include <map>
    # include <iostream>
    # include <fstream>
    # include <string>
    # include "Client.hpp"
    # include <cstdio>
    # include "ParsingHelpers.hpp"

    struct Server;
    class Client;

    class Request
    {
        private:
            std::string RESET; 
            std::string GREEN;
            std::string BLUE; 

            Request();

            void    writeBufferToFile(const std::string& filename, const std::string& decoded);
            // std::string payload;
            std::string fileName;
            void    getFileName();
            std::string base64String;
            std::vector<Server> servers;
            std::string buffer;
            const Server* listeningserver;
            std::string fileToDeleteName;

        public:
            Client* currentClientPtr;
            void    parseHeaders();
            std::map<std::string, std::string>  headers;
            int uploadLocationMatch();
            int deleteLocationMatch();

            void    handleUpload();
            void    handleDelete();
            Request(std::vector<Server> servers, std::string buffer, const Server* listeningserver);
            ~Request();
    };

    #endif