#include "Response.hpp"

#include <unistd.h> 

Response::Response() {
}

template <typename T> std::string Response::to_string(T value) {
    std::ostringstream os;
    os << value;
    return os.str();
}


Response::Response(Request *request, std::vector<Server>& servers,  int i, const Server* listeningserver) : request(request), servers(servers), listeningserver(listeningserver) {
   
    this->i = i;
    statusCode = 200;

    if (request->headers["Method"].empty() || request->headers["Path"].empty() || request->headers["Port"].empty()) {
        Server queriedServer;
        queriedServer._root = "/";
        deliverErrorPage(i, 400, &queriedServer);
    }
    // else 
    //     servePage();
}

void    Response::writeStamp() {
        std::time_t now = std::time(0);
        char* dt = std::ctime(&now);
        dt[strlen(dt) - 1] = '\0';
            
        RESET = "\033[0m";
        RED = "\033[31m";
        BLUE = "\033[34m";
        if (this->statusCode >= 400 && this->statusCode <= 600)
            std::cout << RED << this->listeningserver->ipAddress << " - - [" << dt << "]" << " Server responded with [" << this->statusCode << "]" << RESET << std::endl;
        else
            std::cout << BLUE << this->listeningserver->ipAddress << " - - [" << dt << "]" << " Server responded with [" << this->statusCode << "]" << RESET << std::endl;
}

void    Response::deliverErrorPage(int fd, int statusCode, Server* queriedServer) {
      
        this->statusCode = statusCode;
        std::stringstream sCodeStr;
        sCodeStr << statusCode;
    
    try {
        std::string file_open(queriedServer->_root + "/" + sCodeStr.str() + ".html");
        std::ifstream file(file_open.c_str());
        if (!file.is_open()) 
            throw std::runtime_error("Could not open file");
        std::ostringstream buffer;
        buffer << file.rdbuf();
        std::string fileContent = buffer.str();
        std::string mimeType = getContentType(request->headers["Path"]);
        std::string response = "HTTP/1.1 "+ sCodeStr.str() + " " + queriedServer->statusCodes[sCodeStr.str()].c_str() +"\r\n";
        response += "Content-Type: " + mimeType + "\r\n";
        response += "Content-Length: " + this->to_string(fileContent.size()) + "\r\n";
        // response += "Connection: close\r\n"; 
        response += "\r\n\r\n";
        response += fileContent; 
        write(fd, response.c_str(), response.size());
        fileContent.clear();
        mimeType.clear();
        response.clear();
    } catch (const std::runtime_error& e) {
        std::string fileContent = "DEFAULT SERVER " + sCodeStr.str() + " ERROR PAGE\n" +  queriedServer->statusCodes[sCodeStr.str()].c_str();
        std::string mimeType = getContentType(request->headers["Path"]);
        std::string response = "HTTP/1.1 "+sCodeStr.str() + " " + queriedServer->statusCodes[sCodeStr.str()].c_str() +"\r\n";
        response += "Content-Type: " + mimeType + "\r\n";
        response += "Content-Length: " + this->to_string(fileContent.size()) + "\r\n";
        response += "Connection: close\r\n"; 
         response += "\r\n";
        response += fileContent;
        write(fd, response.c_str(), response.size());
        fileContent.clear();
        mimeType.clear();
        response.clear();
    }
    
 


}

std::string Response::getContentType(const std::string& fileName) {
    size_t pos = fileName.find_last_of(".");
    std::string ext = fileName.substr(pos + 1); 
    if (pos == std::string::npos)
        return "text/html";
        // return "application/octet-stream";
    else if (ext == "html" || ext == "htm")
        return "text/html";
    else if (ext == "jpg" || ext == "jpeg") 
        return "image/jpeg";
    else if (ext == "ico")
        return "image/x-icon"; 
    else if (ext == "png")
        return "image/png";
    else if (ext == "gif")
        return "image/gif";
    else if (ext == "css") 
        return "text/css";
    else if (ext == "js")
        return "application/javascript";
    return "text/html";
}

std::string Response::getExtType(const std::string& fileName) {
    size_t pos = fileName.find_last_of(".");
    std::string ext = fileName.substr(pos + 1); 
    if (pos == std::string::npos)
        return "";
    else
        return ext;
}

// redo!
void Response::servePage() {
    Server* queriedServer = NULL;
    // we need to make the queriedServer = listening server!!
    // // REDO TO FIRST CHECK SERVER NAME, then if no server name match, check ports
    for (size_t x = 0; x < servers.size(); x++) {
        char *portCheck = ft_itoa(this->listeningserver->port); // added this to test
        // char *portCheck = ft_itoa(servers[x].port);
        // Review if strncmp is the best option...strcmp does not work because one length is 4, the other 5...strncmp is a problem if ports are 8001,80011,
        if (std::strncmp(request->headers["Port"].c_str(), portCheck, 4) == 0 ) {
            queriedServer = &servers[x];
            free (portCheck);
            break ;
        }
        free (portCheck);
    }
  
    std::vector<std::string>::iterator it = queriedServer->_allowedMethods.begin();
    while (it != queriedServer->_allowedMethods.end()) {
        if (*it == request->headers["Method"]) {
            break ;
        }
        it++;
        if (it == queriedServer->_allowedMethods.end())
        {
            deliverErrorPage(i, 405, queriedServer);
            writeStamp();
            return ;
        }
    }
    try {
        if (getExtType(request->headers["Path"]) != "sh") {
            std::string file_open(queriedServer->_root + request->headers["Path"]);
            std::ifstream file(file_open.c_str());
            if (!file.is_open()) 
                throw std::runtime_error("Could not open file");
            std::ostringstream buffer;
            buffer << file.rdbuf();
            std::string fileContent = buffer.str();
            std::string mimeType = getContentType(request->headers["Path"]);
            std::string response = "HTTP/1.1 " + this->to_string(this->statusCode) + " OK\r\n";
            response += "Content-Type: " + mimeType + "\r\n";
            response += "Content-Length: " + this->to_string(fileContent.size()) + "\r\n";
            response += "Connection: close\r\n"; 
            response += "\r\n";
            response += fileContent; 
            write(i, response.c_str(), response.size());
            fileContent.clear();
            mimeType.clear();
            response.clear();
        }
        else if (getExtType(request->headers["Path"]) == "sh")
            this->runCGI();
    }
    catch (const std::runtime_error& e) {
        deliverErrorPage(i, 404, queriedServer);
    }
    writeStamp();
    
}

void Response::runCGI() {
        std::map<std::string, std::string> cgiInfo = listeningserver->_cgi;
        std::string file_open(listeningserver->_root + cgiInfo["location"] + request->headers["Path"]);
        std::ifstream file(file_open.c_str());
        if (!file.is_open()) 
            throw std::runtime_error("Could not open file");
        std::ostringstream buffer;
        buffer << file.rdbuf();
        std::string script = buffer.str();
        std::string output;
        int pipefds[2];
        if (pipe(pipefds) == -1)
            throw std::runtime_error("Issue with creating pipes...");
        int pid = fork();
        if (pid == -1)
            throw std::runtime_error("Issue with creating pipes...");
            else if (pid == 0) {
                dup2(pipefds[1], STDOUT_FILENO);  
                const char* bashPath = cgiInfo["cgi-path-bash"].c_str(); 
                std::string scriptPathString = listeningserver->_root + cgiInfo["location"] + request->headers["Path"];
                const char* scriptPath = scriptPathString.c_str();
                const char* args[] = {bashPath, scriptPath, NULL};
                char** env = NULL;
                execve(bashPath,  const_cast<char* const*>(args), env);
            } else {
                close(pipefds[1]);
                char buffer[1024];
                bzero(buffer, sizeof(buffer));
                int bytesRead = 0;
                while ((bytesRead = read(pipefds[0], buffer, sizeof(buffer) - 1)) > 0) {
                    buffer[bytesRead] = '\0';
                }
                if (bytesRead == -1)
                    std::cout << "Internal Error" << std::endl;
                close(pipefds[0]);
                std::string mimeType = getContentType(request->headers["Path"]);
                std::string response = "HTTP/1.1 " + this->to_string(this->statusCode) + " OK\r\n";
                response += "Content-Type: " + mimeType + "\r\n";
                response += "Content-Length: " + this->to_string(1024) + "\r\n";
                response += "Connection: close\r\n"; 
                response += "\r\n";
                response += "<!DOCTYPE html><html><body><H1>";
                response += buffer;
                response += "</h1><body></html>";
                write(this->i, response.c_str(), response.size());
                script.clear();
                mimeType.clear();
                response.clear();
                wait(NULL);
        }

}

Response::~Response() {

}