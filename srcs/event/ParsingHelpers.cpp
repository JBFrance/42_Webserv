
#include "Request.hpp"

void    initialiseHeaders(std::map<std::string, std::string>& headers) {
    headers["Method"] = "";
    headers["Host"] = "";
    headers["Port"] = "";
    headers["Path"] = "";
    headers["ContentDisposition"] = "";
    headers["ContentLength"] = "";
}

void    getMethod(std::map<std::string, std::string>& headers, char** splitBuffer, int i) {
    (void) i;
    if (headers["Method"].empty()) {
        for (int j = 0; (splitBuffer[0][j] && splitBuffer[0][j] != ' ') ; j++)
            headers["Method"] += splitBuffer[0][j];
    }
    else {
        return ;
    }
}

void    getFileToDelete(std::string& fileToDeleteName, char** splitBuffer, int i) {
    int j = 0;
    if  (fileToDeleteName.empty()) { 
        while (j < i) {
            if ((strncmp("File-To-Delete:", splitBuffer[j], strlen("File-To-Delete:"))) == 0 ) {
                int x = strlen("File-To-Delete: ");
                while (splitBuffer[j][x] && splitBuffer[j][x] ) {
                    if (splitBuffer[j][x] == '\r' || splitBuffer[j][x] == '\n')
                        break ;
                    fileToDeleteName += splitBuffer[j][x];
                    x++;
                }
            }
            j++;
        }   
    }

    //   std::cout << "GOT! FILE TO DELETE IS = '" << fileToDeleteName << "'"<< std::endl;
}



void    getContentDisposition(std::map<std::string, std::string>& headers, char** splitBuffer, int i) {
    (void) i;
    if (headers["ContentDisposition"].empty()) {
        int j = 0;
        while (j < i) {
            if ((strncmp("Content-Disposition:", splitBuffer[j], strlen("Content-Disposition:"))) == 0 ) {
                int x = strlen("Content-Disposition: ");
                std::string ContentDispStr = "";
                while (splitBuffer[j][x] && splitBuffer[j][x] != '\n') {
                    ContentDispStr += splitBuffer[j][x];
                    x++;
                }
                headers["ContentDisposition"] = ContentDispStr;
                break ;
            }
            j++;
        }
    }
    else {
        return ;
    }
}



// Create getHost to refactor below code. getHost should match server_name
void    getPath(std::map<std::string, std::string>& headers, char** splitBuffer, int i) {
    int j = 0;
    if (!(headers["Method"].empty())) {
        const char* method = headers["Method"].c_str();
        while (j < i) {
            if (strncmp(method, splitBuffer[j], strlen(method)) == 0 
                && splitBuffer[j][strlen(method)] == ' ' ) {
                int x = strlen(method);
                x++;
                std::string path = "";
                while (splitBuffer[j][x] && splitBuffer[j][x] != ' ') {
                    path += splitBuffer[j][x];
                    x++;
                }
                headers["Path"] = path;
                // std::cout << "FOUND Path= " << strncmp("GET", splitBuffer[j], strlen("GET"))  <<std::endl;
                // headers["Path"] = "GET";
                break ;
            }
            
            j++;
        }
    }
}
// account for server_name!!
void    getPort(std::map<std::string, std::string>& headers, char** splitBuffer, int i) {
    int j = 0;
    if (!(headers["Method"].empty())) {
        const char* host1 = "Host: localhost:";
        const char* host2 = "Host: 127.0.0.1:";
        while (j < i) {
            if ((strncmp(host1, splitBuffer[j], strlen(host1)) == 0 
                && isdigit(splitBuffer[j][strlen(host1)])) || (strncmp(host2, splitBuffer[j], strlen(host2)) == 0 
                && isdigit(splitBuffer[j][strlen(host2)])) ) {
                int x = strlen(host1);
                std::string port = "";
                while (splitBuffer[j][x] && splitBuffer[j][x] != ' ') {
                    port += splitBuffer[j][x];
                    x++;
                }
                headers["Port"] = port;
                break ;
            }
            j++;
        }
    }
}

void clearBuffer(char** splitBuffer, int i) {
    for (int j = 0; j < i ; j++) {
        free(splitBuffer[j]);
    }
        free(splitBuffer);
}