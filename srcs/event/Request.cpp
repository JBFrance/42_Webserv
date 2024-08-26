
#include "Request.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <ctime>
#include "base64.hpp"

Request::Request() {

};

void Request::writeBufferToFile(const std::string& filename, const std::string& decoded) {
    // if config file has 'uploads' location parsed then we should replace '/uploads/' below with relevant location e.g listeningserver->uploadDir
    std::ofstream outputFile(this->listeningserver->_root + this->listeningserver->_uploadDir + "/" + filename.c_str()); // Open file for writing
   
    if (outputFile.is_open()) {
        outputFile << decoded << std::endl; // Write buffer contents to file
        outputFile.close(); // Close the file stream
        std::cout << "File '" << filename << "' uploaded successfully." << std::endl;
    } else {
        std::cerr << "Unable to open file: " << filename << "\nCheck path to write to exists" << std::endl;
        this->currentClientPtr->clientStatusCode = 500;
    }

}


Request::Request(std::vector<Server> servers, std::string buffer, const Server* listeningserver) : servers(servers), buffer(buffer), listeningserver(listeningserver) {
    std::time_t now = std::time(0);
    fileToDeleteName ="";
    char* dt = std::ctime(&now);
    dt[strlen(dt) - 1] = '\0';
    RESET = "\033[0m";
    GREEN = "\033[32m";
    BLUE = "\033[34m";
 
    parseHeaders();

    std::cout << GREEN << this->listeningserver->ipAddress << " - - [" << dt << "] \"" <<this->headers["Method"] << " " << this->headers["Path"] <<"\"" << RESET << std::endl;

    
    (void)servers;
};



void    Request::getFileName() {
    std::string contentDispositionStr = this->headers["ContentDisposition"];
    size_t pos = contentDispositionStr.find("filename=");
    if (pos != std::string::npos) {
        pos += strlen("filename=") ;
        fileName = contentDispositionStr.substr(pos);
        if (!fileName.empty() && fileName[0] == '"') {
            size_t endQuotePos = fileName.find('"', 1);
            if (endQuotePos != std::string::npos) {
                this->fileName = fileName.substr(1,endQuotePos - 1);
            } 
        }
    } else {
        this->fileName = "nofilenamefound";
    }

}

// fix uploadLocation value retrieval
// int    Request::uploadLocationMatch() {
//     if (this->listeningserver->uploadLocation == this->headers["Path"])
//         return 1;
//     return 0;
// }

// int    Request::deleteLocationMatch() {
//     if (this->listeningserver->deleteLocation == this->headers["Path"])
//         return 1;
//     return 0;
// }

// handle error maybe?
void    Request::handleDelete() {
    if (deleteLocationMatch()) {
        try {
            // std::string filePath = this->listeningserver->_root + this->listeningserver->_uploadDir + "/" + fileToDeleteName;
       
            FILE* fileCheck = std::fopen( filePath.c_str(), "r");
            if (fileCheck) {
                if (std::remove(filePath.c_str()) == 0)
                    std::cout << "File '" << fileToDeleteName << " has been successfully deleted" << std::endl;
                else
                    throw std::runtime_error("Unable to delete " + fileToDeleteName);
            }
            else 
                throw std::runtime_error("File Doesn't Exist");
        } catch (const std::runtime_error& e){
                std::cout << "Exception found: " << e.what() << std::endl;
                currentClientPtr->clientStatusCode = 412;
        }
    }
    else
        currentClientPtr->clientStatusCode = 500;
        
    
}

void    Request::handleUpload() {
    // put error handling
    getFileName();

    try {
      
        size_t payloadStart = buffer.find("\r\n\r\n");
        if (payloadStart != std::string::npos) {
            payloadStart += 4;
            this->base64String = buffer.substr(payloadStart);
        }
        std::string decoded = base64_decode(this->base64String);
        writeBufferToFile(this->fileName, decoded);
        std::string emptyBuffer = "";
        decoded.clear();
        decoded.swap(emptyBuffer);
        base64String.clear();
        base64String.swap(emptyBuffer);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error decoding Base64 string: " << e.what() << std::endl;
    }
}

void    Request::parseHeaders() {
    char** splitBuffer = ft_split(buffer.c_str(), '\n');
    int i = 0;

    // the i may not be necessary...
    for (; splitBuffer[i]; i++) {
    }

    initialiseHeaders(this->headers);
    // handle errors here
    if (splitBuffer[0] != NULL) {
        getMethod(this->headers, splitBuffer, i);
        if (this->headers["Method"] == "DELETE")
            getFileToDelete(this->fileToDeleteName, splitBuffer, i);
        getPath(this->headers, splitBuffer, i);
        getPort(this->headers, splitBuffer, i);
        getContentDisposition(this->headers, splitBuffer, i);
    }       
    clearBuffer(splitBuffer, i);
}


Request::~Request()
{
    fileName.clear();
    fileName.shrink_to_fit();
    base64String.clear();
    servers.clear();
    buffer.clear();
    std::string emptyBuffer = "";
    buffer.swap(emptyBuffer);

    
};



